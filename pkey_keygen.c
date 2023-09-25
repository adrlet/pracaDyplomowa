#include <openssl/rand.h>
#include <openssl/rsa.h>

#include "pkey_keygen.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_KEY_GEN_ADDRESS, type, code))

int generateRsaKeys(int count, int bits, EVP_PKEY ** keys, ENGINE * engine)
{
    int i = 0;
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    if(count < 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto generateRsaKeys_end;
    }

    if(bits < 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto generateRsaKeys_end;
    }

    if(keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto generateRsaKeys_end;
    }

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto generateRsaKeys_end;
    }

    for(; i < count; i++)
    {
        iResult = EVP_PKEY_keygen_init(ctx);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
            goto generateRsaKeys_end;
        }
        iResult = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
            goto generateRsaKeys_end;
        }

        keys[i] = NULL;
        iResult = EVP_PKEY_keygen(ctx, keys+i);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
            goto generateRsaKeys_end;
        }
    }

generateRsaKeys_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);
    return i;
}

int generateKeysOnParams(int count, EVP_PKEY * parameters, EVP_PKEY ** keys, ENGINE * engine)
{
    int i = 0;
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    if(count < 1)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto generateKeysOnParams_end;
    }

    if(parameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto generateKeysOnParams_end;
    }

    if(keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto generateKeysOnParams_end;
    }

    ctx = EVP_PKEY_CTX_new(parameters, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto generateKeysOnParams_end;
    }

    for(; i < count; i++)
    {
        iResult = EVP_PKEY_keygen_init(ctx);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 5);
            goto generateKeysOnParams_end;
        }

        keys[i] = NULL;
        iResult = EVP_PKEY_keygen(ctx, keys + i);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 6);
            goto generateKeysOnParams_end;
        }
    }

generateKeysOnParams_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);
    return i;
}

// Nieuzywane

int generateHmac(EVP_PKEY ** pkeys, int count, EVP_MD * algorithm, ENGINE * engine)
{
    int iResult;
    int i = 0;
    unsigned char * buffer = NULL;
    int size;

    if(count < 0)
    {
        //writeErrorCode(
        goto generateHmac_end;
    }

    if(algorithm == NULL)
    {
        //writeErrorCode(
        goto generateHmac_end;
    }

    size = EVP_MD_size(algorithm);
    if(size == 0)
    {
        //writeErrorCode(
        goto generateHmac_end;
    }

    buffer = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*size);
    if(buffer == NULL)
    {
        //writeErrorCode(
        goto generateHmac_end;
    }

    for(; i < count; i++)
    {
        iResult = RAND_bytes(buffer, size);
        if(iResult != 1)
        {
            //writeErrorCode(
            goto generateHmac_end;
        }

        pkeys[i] = EVP_PKEY_new_raw_private_key(EVP_PKEY_HMAC, engine, buffer, size);
        if(pkeys[i] == NULL)
        {
            //writeErrorCode(
            goto generateHmac_end;
        }
    }

generateHmac_end:
    if(buffer == NULL)
        OPENSSL_clear_free(buffer, size);
    return i;
}
