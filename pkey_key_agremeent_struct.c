#include "pkey_key_agremeent_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_KEY_AGREMEENT_STRUCT_ADDRESS, type, code))

struct dhData * setupDh()
{
    struct dhData * dhParameters;

    dhParameters = (struct dhData*)OPENSSL_malloc(sizeof(struct dhData));
    if(dhParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    dhParameters->secret_len = 0;
    dhParameters->key_pair = NULL;
    dhParameters->peer_public_key = NULL;
    dhParameters->secret = NULL;

    return dhParameters;
};

void freeDh(struct dhData * dhParameters)
{
    if(dhParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    if(dhParameters->key_pair != NULL)
        EVP_PKEY_free(dhParameters->key_pair);

    if(dhParameters->peer_public_key != NULL)
        EVP_PKEY_free(dhParameters->peer_public_key);

    if(dhParameters->secret != NULL)
        OPENSSL_clear_free(dhParameters->secret, sizeof(unsigned char)*dhParameters->secret_len);

    dhParameters->secret_len = 0;
}

int provideDhKeys(EVP_PKEY * key_pair, EVP_PKEY * peer_public_key, struct dhData * dhParameters)
{
    if(dhParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        return -1;
    }

    if(key_pair != NULL)
    {
        if(dhParameters->key_pair != NULL)
            EVP_PKEY_free(dhParameters->key_pair);
        dhParameters->key_pair = key_pair;
    }

    if(peer_public_key != NULL)
    {
        if(dhParameters->peer_public_key != NULL)
            EVP_PKEY_free(dhParameters->peer_public_key);
        dhParameters->peer_public_key = peer_public_key;
    }

    return 0;
}
