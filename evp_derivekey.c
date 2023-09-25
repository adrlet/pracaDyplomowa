#include <string.h>
#include <openssl/evp.h>

#include "evp_derivekey.h"
#include "evp_derivekey_struct.h"

#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_DERIVE_KEY_ADDRESS, type, code))

int pbkdf2_deriveKey(struct deriveData * deriveParameters, const char * passphrase, unsigned char * key, unsigned char * iv)
{
    int iResult;
    unsigned char * pbkdf2_out = NULL;

    if(deriveParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    if(passphrase == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(key == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    if(deriveParameters->iv_len > 0 && iv == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    pbkdf2_out = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*(deriveParameters->key_len+deriveParameters->iv_len));
    if(pbkdf2_out == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    iResult = PKCS5_PBKDF2_HMAC(passphrase, strlen(passphrase), deriveParameters->salt, deriveParameters->salt_len,
                                deriveParameters->iterations, deriveParameters->algorithm,
                                deriveParameters->key_len+deriveParameters->iv_len, pbkdf2_out);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    memcpy(key, pbkdf2_out, sizeof(unsigned char)*deriveParameters->key_len);
    memcpy(iv, pbkdf2_out+sizeof(unsigned char)*deriveParameters->key_len, sizeof(unsigned char)*deriveParameters->iv_len);

    iResult = 0;
    goto pbkdf2_deriveKey_end;

error:
    iResult = -1;

pbkdf2_deriveKey_end:
    if(pbkdf2_out != NULL)
        OPENSSL_clear_free(pbkdf2_out, sizeof(unsigned char)*(deriveParameters->key_len+deriveParameters->iv_len));
    return iResult;
}
