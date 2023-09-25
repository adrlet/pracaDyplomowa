#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_async_key.h"
#include "pkey_async_key_struct.h"

#include "pkey_check.h"
#include "pkey_keygen.h"
#include "pkey_priv_loadwrite.h"
#include "pkey_pub_loadwrite.h"
#include "pkey_priv_loadwrite_provider.h"
#include "pkey_param_loadwrite.h"

#include "symmetric_cipher_map.h"
#include "errorLog.h"
#include "bio_pre.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_KEY_GEN_ADDRESS, type, code))

void * async_check(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    printf("async_check\n");

    int iResult = 0;
    int * iResult_alloc = NULL;
    int pkey_algorithm = 0;

    struct async_key_data * data = (struct async_key_data*)arg;

    BIO * pubKeyIO = NULL;
    BIO * privKeyIO = NULL;
    BIO * paramsIO = NULL;

    EVP_PKEY * key = NULL;
    EVP_PKEY * param = NULL;

    struct privPassword * provider = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    provider = setupProvider();
    if(provider == NULL)
        goto error;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->cipherAlgorithm));
    if(iResult == -1)
        goto error;

    pubKeyIO = readFile(data->pubKeyPath);
    if(pubKeyIO == NULL)
        goto error;
    key = importRsaPublicKey(pubKeyIO, NULL);
    if(key == NULL)
        goto error;
    pkey_algorithm = EVP_PKEY_type(EVP_PKEY_id(key));

    switch(pkey_algorithm)
    {
    case EVP_PKEY_RSA:
        privKeyIO = readFile(data->privKeyPath);
        key = importRsaPrivateKey(privKeyIO, &key, provider);
        iResult = checkRsaKeyPair(key);
        if(iResult == -1)
            goto error;
        break;

    case EVP_PKEY_DH:
        paramsIO = readFile(data->paramPath);
        param = loadParameters(NULL, paramsIO);
        iResult = checkDhPubKey(param, key);
        if(iResult == -1)
            goto error;
        break;

    case EVP_PKEY_EC:
        privKeyIO = readFile(data->privKeyPath);
        key = importRsaPrivateKey(privKeyIO, &key, provider);
        iResult = checkEccKeyPair(key);
        if(iResult == -1)
            goto error;
        break;

    case EVP_PKEY_DSA:
        iResult = checkPubKey(key, NULL);
        if(iResult == -1)
            goto error;
    }

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(pubKeyIO != NULL)
        freeBio(pubKeyIO);
    if(privKeyIO != NULL)
        freeBio(privKeyIO);
    if(paramsIO != NULL)
        freeBio(paramsIO);
    if(key != NULL)
        EVP_PKEY_free(key);
    if(param != NULL)
        EVP_PKEY_free(param);
    if(provider != NULL)
        freeProvider(provider);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_generate_rsa_key(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    printf("async_generate_rsa_key\n");

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_key_data * data = (struct async_key_data*)arg;

    BIO * pubKeyIO = NULL;
    BIO * privKeyIO = NULL;

    EVP_PKEY ** key = NULL;

    struct privPassword * provider = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));
    key = (EVP_PKEY**)malloc(sizeof(EVP_PKEY*));

    provider = setupProvider();
    if(provider == NULL)
        goto error;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->cipherAlgorithm));
    if(iResult == -1)
        goto error;

    pubKeyIO = writeFile(data->pubKeyPath);
    if(pubKeyIO == NULL)
        goto error;
    privKeyIO = writeFile(data->privKeyPath);
    if(privKeyIO == NULL)
        goto error;

    iResult = generateRsaKeys(1, data->keySize, key, NULL);
    if(iResult != 1)
        goto error;

    iResult = extractRsaPublicKey(*key, pubKeyIO);
    if(iResult != 0)
        goto error;
    iResult = extractRsaPrivateKey(*key, privKeyIO, provider);
    if(iResult != 0)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(pubKeyIO != NULL)
    {
        BIO_flush(pubKeyIO);
        freeBio(pubKeyIO);
    }
    if(privKeyIO != NULL)
    {
        BIO_flush(privKeyIO);
        freeBio(privKeyIO);
    }
    if(key != NULL)
    {
        if(*key != NULL)
            EVP_PKEY_free(*key);
        free(key);
    }
    if(provider != NULL)
        freeProvider(provider);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_generate_on_param_key(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    printf("async_generate_on_param_key\n");

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_key_data * data = (struct async_key_data*)arg;

    BIO * pubKeyIO = NULL;
    BIO * privKeyIO = NULL;
    BIO * paramIO = NULL;

    EVP_PKEY * key = NULL;
    EVP_PKEY * param = NULL;

    struct privPassword * provider = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    provider = setupProvider();
    if(provider == NULL)
        goto error;
    iResult = provideProviderPassword(provider, data->passphrase, NULL);
    if(iResult == -1)
        goto error;
    iResult = provideProviderAlgorithm(provider, get_symmetric_algorithm(data->cipherAlgorithm));
    if(iResult == -1)
        goto error;

    pubKeyIO = writeFile(data->pubKeyPath);
    if(pubKeyIO == NULL)
        goto error;
    privKeyIO = writeFile(data->privKeyPath);
    if(privKeyIO == NULL)
        goto error;
    paramIO = readFile(data->paramPath);
    if(paramIO == NULL)
        goto error;

    param = loadParameters(NULL, paramIO);

    iResult = generateKeysOnParams(1, param, &key, NULL);
    if(iResult != 1)
        goto error;

    iResult = extractRsaPublicKey(key, pubKeyIO);
    if(iResult != 0)
        goto error;
    iResult = extractRsaPrivateKey(key, privKeyIO, provider);
    if(iResult != 0)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(pubKeyIO != NULL)
    {
        BIO_flush(pubKeyIO);
        freeBio(pubKeyIO);
    }
    if(privKeyIO != NULL)
    {
        BIO_flush(privKeyIO);
        freeBio(privKeyIO);
    }
    if(paramIO != NULL)
        freeBio(paramIO);
    if(key != NULL)
        EVP_PKEY_free(key);
    if(param != NULL)
        EVP_PKEY_free(param);
    if(provider != NULL)
        freeProvider(provider);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
