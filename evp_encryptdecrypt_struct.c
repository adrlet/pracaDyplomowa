#include <string.h>

#include "evp_encryptdecrypt_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ENCRYPT_DECRYPT_STRUCT_ADDRESS, type, code))

struct cipherData * setupCipher()
{
    struct cipherData * cipherParameters = NULL;

    cipherParameters = (struct cipherData*)OPENSSL_malloc(sizeof(struct cipherData));
    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return cipherParameters;
    }

    cipherParameters->key = NULL;
    cipherParameters->iv = NULL;

    cipherParameters->algorithm = NULL;

    cipherParameters->key_len = 0;
    cipherParameters->iv_len = 0;

    return cipherParameters;
};

void freeCipher(struct cipherData * cipherParameters)
{
    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    if(cipherParameters->key_len > 0 && cipherParameters->key != NULL)
        OPENSSL_clear_free(cipherParameters->key, sizeof(unsigned char)*cipherParameters->key_len);
    cipherParameters->key = NULL;
    cipherParameters->key_len = 0;

    if(cipherParameters->iv_len > 0 && cipherParameters->iv != NULL)
        OPENSSL_clear_free(cipherParameters->iv, sizeof(unsigned char)*cipherParameters->iv_len);
    cipherParameters->iv = NULL;
    cipherParameters->iv_len = 0;

    cipherParameters->algorithm = NULL;

    OPENSSL_clear_free(cipherParameters, sizeof(struct cipherData));
}

int provideCipherAlgorithm(struct cipherData * cipherParameters, const EVP_CIPHER * algorithm)
{
    int iResult;

    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }
    cipherParameters->algorithm = algorithm;

    cipherParameters->key_len = EVP_CIPHER_key_length(algorithm);
    if(cipherParameters->key_len == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    cipherParameters->iv_len = EVP_CIPHER_iv_length(algorithm);

    iResult = 0;
    goto provideCipherAlgorithm_end;

error:
    cipherParameters->algorithm = NULL;
    cipherParameters->key_len = 0;
    cipherParameters->iv_len = 0;
    iResult = -1;

provideCipherAlgorithm_end:
    return iResult;
}

int provideCipherKeyIv(struct cipherData * cipherParameters, unsigned char * key, unsigned char * iv)
{
    int iResult;

    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(key == NULL && iv == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(key != NULL)
    {
        if(cipherParameters->key != NULL)
            OPENSSL_clear_free(cipherParameters->key, sizeof(unsigned char)*cipherParameters->key_len);
        cipherParameters->key = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*cipherParameters->key_len);
        if(cipherParameters->key == NULL)
        {
            writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
            goto error;
        }
        memcpy(cipherParameters->key, key, sizeof(unsigned char)*cipherParameters->key_len);
    }

    if(iv != NULL)
    {
        if(cipherParameters->iv != NULL)
            OPENSSL_clear_free(cipherParameters->iv, sizeof(unsigned char)*cipherParameters->iv_len);
        cipherParameters->iv = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*cipherParameters->iv_len);
        if(cipherParameters->iv == NULL)
        {
            writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
            goto error;
        }
        memcpy(cipherParameters->iv, iv, sizeof(unsigned char)*cipherParameters->iv_len);
    }

    iResult = 0;
    goto provideCipherKeyIv_end;

error:
    iResult = -1;

provideCipherKeyIv_end:
    return iResult;
}
