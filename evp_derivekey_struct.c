#include <openssl/rand.h>

#include "evp_derivekey_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_DERIVE_KEY_STRUCT_ADDRESS, type, code))

struct deriveData * setupDerive()
{
    struct deriveData * deriveParamters = NULL;

    deriveParamters = (struct deriveData*)OPENSSL_malloc(sizeof(struct deriveData));
    if(deriveParamters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return deriveParamters;
    }

    deriveParamters->key_len = 0;
    deriveParamters->iv_len = 0;
    deriveParamters->salt_len = 0;

    deriveParamters->iterations = SAFE_ITERATIONS;
    deriveParamters->algorithm = NULL;
    deriveParamters->salt = NULL;

    return deriveParamters;
}

void freeDerive(struct deriveData * deriveParamters)
{
    if(deriveParamters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    if(deriveParamters->salt_len > 0 && deriveParamters->salt != NULL)
        OPENSSL_clear_free(deriveParamters->salt, sizeof(unsigned char)*deriveParamters->salt_len);

    deriveParamters->salt = NULL;
    deriveParamters->salt_len = 0;
    deriveParamters->key_len = 0;
    deriveParamters->iv_len = 0;
    deriveParamters->iterations = SAFE_ITERATIONS;
    deriveParamters->algorithm = NULL;

    OPENSSL_clear_free(deriveParamters, sizeof(struct deriveData));
}

int provideDeriveAlgorithm(struct deriveData * deriveParamters, const EVP_MD * algorithm)
{
    int iResult = 0;

    if(deriveParamters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    deriveParamters->algorithm = algorithm;

    iResult = 0;
    goto provideDeriveAlgorithm_end;

error:
    iResult = -1;

provideDeriveAlgorithm_end:
    return iResult;
}

int provideDeriveCipherInfo(struct deriveData * deriveParameters, int key_len, int iv_len)
{
    int iResult = 0;

    if(deriveParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(key_len == 0 && iv_len == 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    deriveParameters->key_len = key_len;
    deriveParameters->iv_len = iv_len;

    iResult = 0;
    goto provideDeriveCipherInfo_end;

error:
    iResult = -1;

provideDeriveCipherInfo_end:
    return iResult;
}

int provideDeriveIterations(struct deriveData * deriveParameters, int iterations)
{
    if(deriveParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        return -1;
    }

    deriveParameters->iterations = iterations;

    return 0;
}

int generateDeriveSalt(struct deriveData * deriveParameters, int salt_len)
{
    int iResult = 0;

    if(deriveParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 6);
        goto generateDeriveSalt_end;
    }

    if(deriveParameters->salt != NULL && deriveParameters->salt_len > 0)
    {
        OPENSSL_clear_free(deriveParameters->salt, sizeof(unsigned char)*deriveParameters->salt_len);
        deriveParameters->salt_len = 0;
    }

    deriveParameters->salt_len = (salt_len > 0 ? salt_len : SALT_LEN);
    deriveParameters->salt = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*deriveParameters->salt_len);
    if(deriveParameters->salt == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
        goto error;
    }

    iResult = RAND_bytes(deriveParameters->salt, deriveParameters->salt_len*sizeof(unsigned char));
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    iResult = 0;
    goto generateDeriveSalt_end;

error:
    if(deriveParameters->salt != NULL)
        OPENSSL_clear_free(deriveParameters->salt, sizeof(unsigned char)*deriveParameters->salt_len);
    deriveParameters->salt_len = 0;

    iResult = -1;

generateDeriveSalt_end:
    return iResult;
}
