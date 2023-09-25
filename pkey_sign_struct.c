#include "pkey_sign_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_SIGN_STRUCT_ADDRESS, type, code))

struct signData * setupSign()
{
    struct signData * signParameters;

    signParameters = (struct signData*)OPENSSL_malloc(sizeof(struct signData));
    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    signParameters->algorithm = NULL;
    signParameters->key_pair = NULL;
    signParameters->digest_size = 0;

    return signParameters;
};

void freeSign(struct signData * signParameters)
{
    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    signParameters->algorithm = NULL;
    if(signParameters->key_pair != NULL)
        EVP_PKEY_free(signParameters->key_pair);
    signParameters->key_pair = NULL;
    signParameters->digest_size = 0;
}

int provideSignDigest(EVP_MD * algorithm, struct signData * signParameters)
{
    int iResult;

    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    signParameters->algorithm = algorithm;
    signParameters->digest_size = (EVP_MD_size(algorithm) > EVP_MD_block_size(algorithm) ? EVP_MD_size(algorithm) : EVP_MD_block_size(algorithm));
    if(signParameters->digest_size == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    iResult = 0;
    goto provideSignDigest_end;

error:
    iResult = -1;
    signParameters->algorithm = NULL;
    signParameters->digest_size = 0;

provideSignDigest_end:
    return iResult;
}

int provideSignKeys(EVP_PKEY * pair, struct signData * signParameters)
{
    int iResult;

    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(pair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }
    if(signParameters->key_pair != NULL)
        EVP_PKEY_free(signParameters->key_pair);
    signParameters->key_pair = pair;

    iResult = 0;
    goto provideSignKeys_end;

error:
    iResult = -1;

provideSignKeys_end:
    return iResult;
}
