#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_async_sign.h"
#include "pkey_async_sign_struct.h"
#include "asymmetricSignPanel_struct.h"

#include "pkey_sign.h"
#include "pkey_sign_struct.h"
#include "pkey_priv_loadwrite.h"
#include "pkey_priv_loadwrite_provider.h"
#include "pkey_pub_loadwrite.h"
#include "pkey_param_loadwrite.h"

#include "errorLog.h"
#include "bio_pre.h"
#include "message_digest_map.h"
#include "symmetric_cipher_map.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_SIGN_ADDRESS, type, code))

void * async_sign(void * arg)
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

    struct async_sign_data * data = (struct async_sign_data*)arg;

    BIO * input = NULL;
    BIO * output = NULL;
    BIO * keyIO = NULL;

    EVP_PKEY * key = NULL;

    struct privPassword * provider = NULL;
    struct signData * sign_data = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    input = readFile(data->inputPath);
    if(input == NULL)
        goto error;

    if(data->flags & SIGN_BASE64)
        output = writeFile_Base64(data->outputPath);
    else
        output = writeFile(data->outputPath);
    if(output == NULL)
        goto error;

    keyIO = readFile(data->keyPath);
    if(keyIO == NULL)
        goto error;

    provider = setupProvider();
    if(provider == NULL)
        goto error;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->cipher));
    if(iResult == -1)
        goto error;

    key = importRsaPrivateKey(keyIO, NULL, provider);
    if(key == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(key));
    if(type != EVP_PKEY_EC && type != EVP_PKEY_DSA)
        goto error;

    if(type == EVP_PKEY_EC)
        printf("EC\n");
    else
        printf("DSA\n");

    sign_data = setupSign();
    if(sign_data == NULL)
        goto error;

    iResult = provideSignDigest(get_digest_algorithm(data->digest), sign_data);
    if(iResult != 0)
        goto error;

    iResult = provideSignKeys(key, sign_data);
    if(iResult != 0)
        goto error;

    iResult = sign(input, output, sign_data, data->handler, NULL);
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
    printf("0\n");
    if(output != NULL)
    {
        BIO_flush(output);
        freeBio(output);
    }
    printf("1\n");
    if(keyIO != NULL)
        freeBio(keyIO);
    printf("2\n");
    //if(key != NULL)
        //EVP_PKEY_free(key);
    printf("3\n");
    if(sign_data != NULL)
        freeSign(sign_data);
    printf("4\n");
    if(provider != NULL)
        freeProvider(provider);
    printf("5\n");

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_verify(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    printf("async_verify\n");

    int iResult = 0;
    int equal = 0;
    int * iResult_alloc = NULL;
    int type = 0;

    struct async_sign_data * data = (struct async_sign_data*)arg;

    BIO * input = NULL;
    BIO * signInput = NULL;
    BIO * keyIO = NULL;

    EVP_PKEY * key = NULL;

    struct signData * sign_data = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    input = readFile(data->inputPath);
    if(input == NULL)
        goto error;

    if(data->flags & SIGN_BASE64)
        signInput = readFile_Base64(data->outputPath);
    else
        signInput = readFile(data->outputPath);
    if(signInput == NULL)
        goto error;

    keyIO = readFile(data->keyPath);
    if(keyIO == NULL)
        goto error;

    key = importRsaPublicKey(keyIO, NULL);
    if(key == NULL)
        goto error;

    type = EVP_PKEY_type(EVP_PKEY_id(key));
    if(type != EVP_PKEY_EC && type != EVP_PKEY_DSA)
        goto error;

    sign_data = setupSign();
    if(sign_data == NULL)
        goto error;

    iResult = provideSignDigest(get_digest_algorithm(data->digest), sign_data);
    if(iResult != 0)
        goto error;

    iResult = provideSignKeys(key, sign_data);
    if(iResult != 0)
        goto error;

    iResult = asymmetricVerify(input, signInput, sign_data, data->handler, NULL, &equal);
    if(iResult < 0 || equal != 1)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    data->handler->interrupt = 1;

    if(input != NULL)
        freeBio(input);
    if(signInput != NULL)
        freeBio(signInput);
    if(keyIO != NULL)
        freeBio(keyIO);
    if(sign_data != NULL)
        freeSign(sign_data);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
