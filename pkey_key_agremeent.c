#include "pkey_key_agremeent.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_KEY_AGREMEENT_ADDRESS, type, code))

int keyAgreement(struct dhData * dhParameters, ENGINE * engine)
{
    int iResult;
    EVP_PKEY_CTX * ctx = NULL;

    ctx = EVP_PKEY_CTX_new(dhParameters->key_pair, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    iResult = EVP_PKEY_derive_init(ctx);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    iResult = EVP_PKEY_derive_set_peer(ctx, dhParameters->peer_public_key);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    iResult = EVP_PKEY_derive(ctx, NULL, (size_t*)&(dhParameters->secret_len));
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
        goto error;
    }

    dhParameters->secret = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*dhParameters->secret_len);
    if(dhParameters->secret == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    // EVP_PKEY_derive zeros passed buffer pointer, it wont affect address pointer but it will affect address from array
    iResult = EVP_PKEY_derive(ctx, dhParameters->secret, (size_t*)&(dhParameters->secret_len));
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }

    iResult = 0;
    goto keyAgreement_end;

error:
    iResult = -1;

keyAgreement_end:
    if(ctx != NULL)
        EVP_PKEY_CTX_free(ctx);

    return iResult;
}
