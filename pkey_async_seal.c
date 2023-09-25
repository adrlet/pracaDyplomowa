#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_async_seal.h"
#include "pkey_async_seal_struct.h"
#include "asymmetricSealPanel_struct.h"

#include "pkey_envelope.h"
#include "pkey_envelope_struct.h"
#include "pkey_priv_loadwrite.h"
#include "pkey_priv_loadwrite_provider.h"
#include "pkey_pub_loadwrite.h"
#include "pkey_param_loadwrite.h"

#include "errorLog.h"
#include "bio_pre.h"
#include "message_digest_map.h"
#include "symmetric_cipher_map.h"
#include "thread_asynchronichandler.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_SEAL_ADDRESS, type, code))

void * async_seal(void * arg)
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
    int i = 0;
    int j = 0;

    struct async_seal_data * data = (struct async_seal_data*)arg;

    BIO * input = NULL;
    BIO * output = NULL;
    BIO * pubKeyIO = NULL;
    BIO * ivIO = NULL;
    BIO * keyIO = NULL;
    const char * KeyName = NULL;

    EVP_PKEY * pubKey = NULL;
    EVP_PKEY ** pubKeys = NULL;

    struct envelopeData * envelope_data = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    input = readFile(data->inputPath);
    if(input == NULL)
        goto error;

    if(data->flags & SEAL_BASE64)
    {
        output = writeFile_Base64(data->outputPath);
        ivIO = writeFile_Base64(data->ivPath);
    }
    else
    {
        output = writeFile(data->outputPath);
        ivIO = writeFile(data->ivPath);
    }

    if(output == NULL)
        goto error;
    if(ivIO == NULL)
        goto error;

    pubKeys = (EVP_PKEY**)malloc(sizeof(EVP_PKEY*)*data->keynum);
    for(i = 0; i < data->keynum; i++)
    {
        keyIO = readFile(data->pubKeysPath[i]);
        if(keyIO == NULL)
            goto error;
        pubKeys[i] = importRsaPublicKey(keyIO, NULL);
        freeBio(keyIO);

        if(pubKeys[i] == NULL)
            continue;
        type = EVP_PKEY_type(EVP_PKEY_id(pubKeys[i]));
        if(type != EVP_PKEY_RSA)
            goto error;
    }

    envelope_data = setupEnvelope();
    if(envelope_data == NULL)
        goto error;

    iResult = provideEnvelopeAlgorithm(envelope_data, get_symmetric_algorithm(data->cipher));
    if(iResult != 0)
        goto error;

    iResult = provideEnvelopeKeys(envelope_data, pubKeys, data->keynum);
    if(iResult != 0)
        goto error;

    iResult = envelopeSeal(input, envelope_data, output);
    if(iResult != 0)
        goto error;

    BIO_write(ivIO, envelope_data->iv, envelope_data->iv_len);
    BIO_flush(ivIO);

    j = strlen(data->keyPath);
    KeyName = (char*)malloc(sizeof(char)*(j+1+24));
    strcpy(KeyName, data->keyPath);
    for(i = 0; i < data->keynum; i++)
    {
        sprintf(KeyName+j, "%d", i);
        if(data->flags & SEAL_BASE64)
            keyIO = writeFile_Base64(KeyName);
        else
            keyIO = writeFile(KeyName);
        BIO_write(keyIO, envelope_data->encrypted_keys[i], envelope_data->encrypted_key_len[i]);
        BIO_flush(keyIO);
        freeBio(keyIO);
    }

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    data->handler->interrupt = 1;

    if(input != NULL)
        freeBio(input);
    if(output != NULL)
    {
        BIO_flush(output);
        freeBio(output);
    }
    if(ivIO != NULL)
        freeBio(ivIO);
    if(envelope_data != NULL)
        freeEnvelope(envelope_data);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_open(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    printf("async_sign\n");

    int iResult = 0;
    int * iResult_alloc = NULL;
    int type = 0;
    int i = 0;
    int j = 0;

    struct async_seal_data * data = (struct async_seal_data*)arg;

    BIO * input = NULL;
    BIO * output = NULL;
    BIO * privKeyIO = NULL;
    BIO * ivIO = NULL;
    BIO * keyIO = NULL;

    EVP_PKEY * privKey = NULL;

    struct privPassword * provider = NULL;
    struct envelopeData * envelope_data = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    output = writeFile(data->outputPath);
    if(output == NULL)
        goto error;

    if(data->flags & SEAL_BASE64)
    {
        input = readFile_Base64(data->inputPath);
        ivIO = readFile_Base64(data->ivPath);
        keyIO = readFile_Base64(data->keyPath);
    }
    else
    {
        input = readFile(data->inputPath);
        ivIO = readFile(data->ivPath);
        keyIO = readFile(data->keyPath);
    }
    if(input == NULL)
        goto error;
    if(ivIO == NULL)
        goto error;
    if(keyIO == NULL)
        goto error;

    privKeyIO = readFile(data->privKeyPath);
    if(privKeyIO == NULL)
        goto error;

    provider = setupProvider();
    if(provider == NULL)
        goto error;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->privCipher));
    if(iResult == -1)
        goto error;

    privKey = importRsaPrivateKey(privKeyIO, NULL, provider);
    if(privKey == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(privKey));
    if(type != EVP_PKEY_RSA)
        goto error;

    envelope_data = setupEnvelope();
    if(envelope_data == NULL)
        goto error;

    iResult = provideEnvelopeAlgorithm(envelope_data, get_symmetric_algorithm(data->cipher));
    if(iResult != 0)
        goto error;

    iResult = provideEnvelopeKeys(envelope_data, &privKey, data->keynum);
    if(iResult != 0)
        goto error;

    envelope_data->encrypted_key_len[0] = EVP_PKEY_size(privKey);
    BIO_read(ivIO, envelope_data->iv, envelope_data->iv_len);
    BIO_read(keyIO, envelope_data->encrypted_keys[0], EVP_PKEY_size(privKey));

    char largebuffer[128];
    iResult = envelopeOpen(input, envelope_data, output);
    if(iResult != 0)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    data->handler->interrupt = 1;

    if(input != NULL)
        freeBio(input);
    if(output != NULL)
    {
        BIO_flush(output);
        freeBio(output);
    }
    if(ivIO != NULL)
        freeBio(ivIO);
    //if(key != NULL)
        //EVP_PKEY_free(key);
    if(envelope_data != NULL)
        freeEnvelope(envelope_data);
    if(provider != NULL)
        freeProvider(provider);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
