#include <openssl/rsa.h>
#include <openssl/dh.h>
#include <openssl/ec.h>

#include "pkey_check.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_CHECK_ADDRESS, type, code))

int checkRsaKeyPair(EVP_PKEY * key_pair)
{
    int iResult;
    RSA * rsaPair = NULL;

    if(key_pair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    rsaPair = EVP_PKEY_get0_RSA(key_pair);
    if(rsaPair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    iResult = RSA_check_key(rsaPair);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    iResult = 0;
    goto checkRsaKeyPair_end;

error:
    iResult = -1;

checkRsaKeyPair_end:
    return iResult;
}

int checkDhPubKey(EVP_PKEY * params, EVP_PKEY * pub_key)
{
    int iResult;
    DH * dhParams;
    DH * dhPubKey;
    BIGNUM * bnPubKey;

    if(params == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(pub_key == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    dhParams = EVP_PKEY_get0_DH(params);
    if(dhParams == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    dhPubKey = EVP_PKEY_get0_DH(pub_key);
    if(dhPubKey == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
        goto error;
    }

    bnPubKey = DH_get0_pub_key(dhPubKey);
    if(bnPubKey == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }

    iResult = DH_check_pub_key_ex(dhParams, bnPubKey);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 5);
        goto error;
    }

    iResult = 0;
    goto checkDhPubKey_end;

error:
    iResult = -1;

checkDhPubKey_end:
    return iResult;
}

int checkDhParams(EVP_PKEY * params)
{
    int iResult;
    DH * dhParams;

    if(params == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    dhParams = EVP_PKEY_get0_DH(params);
    if(dhParams == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 6);
        goto error;
    }

    iResult = DH_check_ex(dhParams);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 7);
        goto error;
    }

    iResult = 0;
    goto checkDhParams_end;

error:
    iResult = -1;

checkDhParams_end:
    return iResult;
}

int checkEccKeyPair(EVP_PKEY * key_pair)
{
    int iResult;
    EC_KEY * eccPair;

    if(key_pair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    eccPair = EVP_PKEY_get0_EC_KEY(key_pair);
    if(eccPair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 8);
        goto error;
    }

    iResult = EC_KEY_check_key(eccPair);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 9);
        goto error;
    }

    iResult = 0;
    goto checkEccKeyPair_end;

error:
    iResult = -1;

checkEccKeyPair_end:
    return iResult;
}

int checkKey(EVP_PKEY * pairKey, ENGINE * engine)
{
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    ctx = EVP_PKEY_CTX_new(pairKey, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 10);
        goto error;
    }

    iResult = EVP_PKEY_check(ctx);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 11);
        goto error;
    }

    iResult = 0;
    goto checkKey_end;

error:
    iResult = -1;

checkKey_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);
    return iResult;
}

int checkPubKey(EVP_PKEY * pubKey, ENGINE * engine)
{
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    ctx = EVP_PKEY_CTX_new(pubKey, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 12);
        goto error;
    }

    iResult = EVP_PKEY_public_check(ctx);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 13);
        goto error;
    }

    iResult = 0;
    goto checkKey_end;

error:
    iResult = -1;

checkKey_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);
    return iResult;
}

int checkParams(EVP_PKEY * params, ENGINE * engine)
{
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    ctx = EVP_PKEY_CTX_new(params, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 14);
        goto error;
    }

    iResult = EVP_PKEY_param_check(ctx);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 15);
        goto error;
    }

    iResult = 0;
    goto checkKey_end;

error:
    iResult = -1;

checkKey_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);
    return iResult;
}
