#include <openssl/bio.h>
#include <openssl/pem.h>

#include "pkey_combine.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_KEY_COMBINE_ADDRESS, type, code))

int combineKeys(EVP_PKEY * public_key, EVP_PKEY * private_key, EVP_PKEY ** key_pair)
{
    int iResult;
    BIO * mem_bio = NULL;

    if(key_pair == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    if(public_key == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(private_key == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    mem_bio = BIO_new(BIO_s_secmem());
    if(mem_bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 0);
        goto error;
    }

    iResult = PEM_write_bio_PUBKEY(mem_bio, public_key);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    (*key_pair) = PEM_read_bio_PUBKEY(mem_bio, key_pair, NULL, NULL);
    if((*key_pair) == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    iResult = PEM_write_bio_PKCS8PrivateKey(mem_bio, private_key, NULL, NULL, 0, NULL, NULL);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    (*key_pair) = PEM_read_bio_PrivateKey(mem_bio, key_pair, NULL, NULL);
    if((*key_pair) == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
        goto error;
    }

    iResult = 0;
    goto combineKeys_end;

error:
    iResult = -1;
    if((*key_pair) != NULL)
        EVP_PKEY_free(*key_pair);

combineKeys_end:
    if(mem_bio != NULL)
        BIO_free(mem_bio);

    return iResult;
}
