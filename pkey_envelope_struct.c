#include "pkey_envelope_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ENVELOPE_STRUCT_ADDRESS, type, code))

struct envelopeData * setupEnvelope()
{
    struct envelopeData * envelopeParameters;

    envelopeParameters = (struct envelopeData*)OPENSSL_malloc(sizeof(struct envelopeData));
    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    envelopeParameters->key_num = 0;
    envelopeParameters->block_len = 0;
    envelopeParameters->iv_len = 0;
    envelopeParameters->pub_keys = NULL;
    envelopeParameters->algorithm = NULL;
    envelopeParameters->encrypted_keys = NULL;
    envelopeParameters->encrypted_key_len = NULL;
    envelopeParameters->iv = NULL;

    return envelopeParameters;
};

void freeEnvelope(struct envelopeData * envelopeParameters)
{
    int i;

    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    envelopeParameters->algorithm = NULL;

    if(envelopeParameters->key_num > 0)
    {
        for(i = 0; i < envelopeParameters->key_num; i++)
        {
            if(envelopeParameters->pub_keys != NULL && envelopeParameters->pub_keys[i] != NULL)
                EVP_PKEY_free(envelopeParameters->pub_keys[i]);
            if(envelopeParameters->encrypted_keys != NULL && envelopeParameters->encrypted_keys[i] != NULL)
                OPENSSL_clear_free(envelopeParameters->encrypted_keys[i], sizeof(unsigned char)*envelopeParameters->encrypted_key_len[i]);
        }
        if(envelopeParameters->pub_keys != NULL)
            OPENSSL_clear_free(envelopeParameters->pub_keys, sizeof(EVP_PKEY*)*envelopeParameters->key_num);
        if(envelopeParameters->encrypted_keys != NULL)
            OPENSSL_clear_free(envelopeParameters->encrypted_keys, sizeof(unsigned char*)*envelopeParameters->key_num);
        if(envelopeParameters->encrypted_key_len != NULL)
            OPENSSL_clear_free(envelopeParameters->encrypted_key_len, sizeof(int)*envelopeParameters->key_num);
    }
    if(envelopeParameters->iv_len > 0 && envelopeParameters->iv != NULL)
        OPENSSL_clear_free(envelopeParameters->iv, sizeof(unsigned char)*envelopeParameters->iv_len);

    envelopeParameters->block_len = 0;
    envelopeParameters->iv_len = 0;
    envelopeParameters->key_num = 0;

    OPENSSL_clear_free(envelopeParameters, sizeof(struct envelopeData));
}

int provideEnvelopeAlgorithm(struct envelopeData * envelopeParameters, EVP_CIPHER * algorithm)
{
    int iResult;

    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }
    envelopeParameters->algorithm = algorithm;

    envelopeParameters->block_len = EVP_CIPHER_block_size(algorithm);
    if(envelopeParameters->block_len == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    envelopeParameters->iv_len = EVP_CIPHER_iv_length(algorithm);
    if(envelopeParameters->iv_len == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    envelopeParameters->iv = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->iv_len);
    if(envelopeParameters->iv == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
        goto error;
    }

    iResult = 0;
    goto provideEnvelopeAlgorithm_end;

error:
    iResult = -1;

provideEnvelopeAlgorithm_end:
    if(iResult == -1)
    {
        envelopeParameters->algorithm = NULL;
        envelopeParameters->block_len = 0;
        if(envelopeParameters->iv != NULL)
            OPENSSL_clear_free(envelopeParameters->iv, sizeof(unsigned char)*envelopeParameters->iv_len);
        envelopeParameters->iv = NULL;
        envelopeParameters->iv_len = 0;
        return -1;
    }

    return iResult;
}

int provideEnvelopeKeys(struct envelopeData * envelopeParameters, EVP_PKEY ** keys, int count)
{
    int iResult;
    int i;

    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(count < 1)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto error;
    }

    envelopeParameters->pub_keys = (EVP_PKEY**)OPENSSL_malloc(sizeof(EVP_PKEY*)*count);
    if(envelopeParameters->pub_keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
        goto error;
    }

    envelopeParameters->encrypted_keys = (unsigned char**)OPENSSL_malloc(sizeof(unsigned char*)*count);
    if(envelopeParameters->encrypted_keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 3);
        goto error;
    }

    envelopeParameters->encrypted_key_len = (int*)OPENSSL_malloc(sizeof(int)*count);
    if(envelopeParameters->encrypted_keys == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 4);
        goto error;
    }

    for(i = 0; i < count; i++)
    {
        if(keys[i] == NULL)
        {
            writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 6);
            goto error;
        }
        envelopeParameters->pub_keys[i] = keys[i];

        envelopeParameters->encrypted_key_len[i] = EVP_PKEY_size(keys[i]);
        if(envelopeParameters->encrypted_key_len == 0)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
            goto error;
        }

        envelopeParameters->encrypted_keys[i] = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->encrypted_key_len[i]);
        if(envelopeParameters->encrypted_keys[i] == NULL)
        {
            writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 5);
            goto error;
        }
    }
    envelopeParameters->key_num = count;

    iResult = 0;
    goto provideEnvelopeKeys_end;

error:
    iResult = -1;

    if(envelopeParameters->encrypted_keys != NULL && envelopeParameters->encrypted_key_len != NULL)
        for(; i >= 0; i--)
            OPENSSL_clear_free(envelopeParameters->encrypted_keys[i], sizeof(unsigned char)*envelopeParameters->encrypted_key_len[i]);
    if(envelopeParameters->encrypted_key_len != NULL)
        OPENSSL_clear_free(envelopeParameters->encrypted_key_len, sizeof(int)*count);
    if(envelopeParameters->encrypted_keys != NULL)
        OPENSSL_clear_free(envelopeParameters->encrypted_keys, sizeof(unsigned char*)*count);
    if(envelopeParameters->pub_keys != NULL)
        OPENSSL_clear_free(envelopeParameters->pub_keys, sizeof(EVP_PKEY*)*count);

provideEnvelopeKeys_end:

    return iResult;
}
