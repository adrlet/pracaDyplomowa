#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_async_agremeent.h"
#include "pkey_async_agremeent_struct.h"
#include "asymmetricAgremeentPanel_struct.h"

#include "pkey_key_agremeent.h"
#include "pkey_async_sign_struct.h"
#include "pkey_pub_loadwrite.h"
#include "pkey_priv_loadwrite.h"
#include "pkey_priv_loadwrite_provider.h"

#include "errorLog.h"
#include "bio_pre.h"
#include "message_digest_map.h"
#include "symmetric_cipher_map.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_AGREMEENT_ADDRESS, type, code))

void * async_agremeent(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    printf("async_sign\n");

    int iResult = 0;
    int * iResult_alloc = NULL;
    int type = 0;

    struct async_agremeent_data * data = (struct async_agremeent_data*)arg;

    BIO * output = NULL;
    BIO * keyIO = NULL;
    BIO * privKeyIO = NULL;
    BIO * pubKeyIO = NULL;

    EVP_PKEY * key = NULL;
    EVP_PKEY * privKey = NULL;
    EVP_PKEY * pubKey = NULL;

    struct privPassword * provider = NULL;
    struct dhData * dh_data = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    keyIO = readFile(data->keyPath);
    if(keyIO == NULL)
        goto error;

    privKeyIO = readFile(data->privKeyPath);
    if(privKeyIO == NULL)
        goto error;

    pubKeyIO = readFile(data->pubKeyPath);
    if(pubKeyIO == NULL)
        goto error;

    if(data->flags & AGREMEENT_BASE64)
        output = writeFile_Base64(data->outputPath);
    else
        output = writeFile(data->outputPath);
    if(output == NULL)
        goto error;

    provider = setupProvider();
    if(provider == NULL)
        return;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->cipher));
    if(iResult == -1)
        goto error;

    key = importRsaPublicKey(keyIO, NULL);
    if(key == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(key));
    if(type != EVP_PKEY_EC && type != EVP_PKEY_DH)
        goto error;

    pubKey = importRsaPublicKey(pubKeyIO, NULL);
    if(pubKey == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(pubKey));
    if(type != EVP_PKEY_EC && type != EVP_PKEY_DH)
        goto error;

    privKey = importRsaPrivateKey(privKeyIO, &pubKey, provider);
    if(pubKey == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(pubKey));
    if(type != EVP_PKEY_EC && type != EVP_PKEY_DH)
        goto error;

    dh_data = setupDh();
    if(dh_data == NULL)
        goto error;
    iResult = provideDhKeys(privKey, key, dh_data);
    if(iResult == -1)
        goto error;

    iResult = keyAgreement(dh_data, NULL);
    if(iResult != 0)
        goto error;

    BIO_write(output, dh_data->secret, dh_data->secret_len*sizeof(unsigned char));
    BIO_flush(output);

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(keyIO != NULL)
        freeBio(keyIO);
    if(pubKeyIO)
        freeBio(pubKeyIO);
    if(privKeyIO)
        freeBio(privKeyIO);
    if(output != NULL)
        BIO_flush(output);
    if(dh_data != NULL)
        freeDh(dh_data);
    if(provider != NULL)
        freeProvider(provider);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
