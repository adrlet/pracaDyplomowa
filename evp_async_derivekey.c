#include <openssl/bio.h>

#include "evp_async_derivekey.h"
#include "evp_async_derivekey_struct.h"
#include "evp_derivekey.h"
#include "evp_derivekey_struct.h"
#include "evp_salt.h"
#include "symmetricEncrypt_struct.h"

#include "message_digest_map.h"
#include "bio_errorhandle.h"
#include "bio_pre.h"
#include "errorLog.h"
#include "settings.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_DERIVE_KEY_ADDRESS, type, code))

void * async_derive_key(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_deriveKey_data * deriveKey_data = NULL;
    struct deriveData * deriveParams = NULL;

    BIO * bio = NULL;

    deriveKey_data = (struct async_deriveKey_data*)arg;

    iResult_alloc = (int*)malloc(sizeof(int));
    if(iResult_alloc == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    deriveParams = setupDerive();
    if(deriveParams == NULL)
        goto error;

    iResult = provideDeriveAlgorithm(deriveParams, get_digest_algorithm((int)settings_get_derive_algorithm()));
    if(iResult == -1)
        goto error;

    iResult = provideDeriveCipherInfo(deriveParams, deriveKey_data->keylen, deriveKey_data->ivlen);
    if(iResult == -1)
        goto error;

    if(deriveKey_data->flags & SALT)
    {
        iResult = generateDeriveSalt(deriveParams, deriveKey_data->saltlen);
        if(iResult == -1)
            goto error;

        if(deriveKey_data->flags & SYMMETRIC_MODE_DECRYPT)
        {
            if(deriveKey_data->flags & BASE64)
                bio = readFile_Base64(deriveKey_data->saltpath);
            else
                bio = readFile(deriveKey_data->saltpath);
            if(bio == NULL)
                goto error;

            iResult = readSalt(bio, deriveParams->salt, deriveParams->salt_len);
            if(iResult == -1)
                goto error;
        }
        else
        {
            if(deriveKey_data->flags & BASE64)
                bio = writeFile_Base64(deriveKey_data->saltpath);
            else
                bio = writeFile(deriveKey_data->saltpath);
            if(bio == NULL)
                goto error;

            iResult = writeSalt(bio, deriveParams->salt, deriveParams->salt_len, 1);
            if(iResult == -1)
                goto error;
        }
    }

    iResult = pbkdf2_deriveKey(deriveParams, deriveKey_data->passphrase, deriveKey_data->key, deriveKey_data->iv);
    if(iResult == -1)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(deriveParams != NULL)
        freeDerive(deriveParams);

    if(bio != NULL)
        freeBio(bio);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
