#include <string.h>

#include "evp_async_derivekey_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_DERIVE_KEY_STRUCT_ADDRESS, type, code))

struct async_deriveKey_data * async_deriveKey_data_new()
{
    struct async_deriveKey_data * deriveKey_data = NULL;

    deriveKey_data = (struct async_deriveKey_data*)malloc(sizeof(struct async_deriveKey_data));
    if(deriveKey_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    deriveKey_data->passphrase = NULL;
    deriveKey_data->key = NULL;
    deriveKey_data->iv = NULL;
    deriveKey_data->saltpath = NULL;

    deriveKey_data->digest_algorithm_position = 0;
    deriveKey_data->keylen = 0;
    deriveKey_data->ivlen = 0;
    deriveKey_data->saltlen = 0;

    deriveKey_data->flags = 0;

end:
    return deriveKey_data;
};

int async_deriveKey_data_delete(struct async_deriveKey_data * deriveKey_data)
{
    int iResult = 0;

    if(deriveKey_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    deriveKey_data->passphrase = NULL;

    if(deriveKey_data->key != NULL && deriveKey_data->keylen > 0)
    {
        memset(deriveKey_data->key, 0, deriveKey_data->keylen*sizeof(unsigned char));
        free(deriveKey_data->key);
    }
    deriveKey_data->key = NULL;
    deriveKey_data->keylen = 0;

    if(deriveKey_data->iv != NULL && deriveKey_data->ivlen > 0)
    {
        memset(deriveKey_data->iv, 0, deriveKey_data->ivlen*sizeof(unsigned char));
        free(deriveKey_data->iv);
    }
    deriveKey_data->iv = NULL;
    deriveKey_data->ivlen = 0;

    deriveKey_data->saltpath = NULL;
    deriveKey_data->saltlen = 0;

    deriveKey_data->digest_algorithm_position = 0;
    deriveKey_data->flags = 0;

    free(deriveKey_data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_deriveKey_data_provide(struct async_deriveKey_data * deriveKey_data, char * passphrase, const char * saltpath,
                                 int digest_algorithm_position, int keylen, int ivlen, int saltlen, unsigned char flags)
{
    int iResult = 0;

    if(deriveKey_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    deriveKey_data->passphrase = passphrase;
    deriveKey_data->saltpath = saltpath;

    deriveKey_data->digest_algorithm_position = digest_algorithm_position;
    deriveKey_data->keylen = keylen;
    deriveKey_data->ivlen = ivlen;
    deriveKey_data->saltlen = saltlen;

    deriveKey_data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_deriveKey_data_init_buffers(struct async_deriveKey_data * deriveKey_data)
{
    int iResult = 0;

    if(deriveKey_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    if(deriveKey_data->keylen > 0 && deriveKey_data->key == NULL)
    {
        deriveKey_data->key = (unsigned char*)malloc(sizeof(unsigned char)*deriveKey_data->keylen);
        if(deriveKey_data->key == NULL)
        {
            //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
            goto error;
        }
        memset(deriveKey_data->key, 0, sizeof(unsigned char)*deriveKey_data->keylen);
    }

    if(deriveKey_data->ivlen > 0 && deriveKey_data->iv == NULL)
    {
        deriveKey_data->iv = (unsigned char*)malloc(sizeof(unsigned char)*deriveKey_data->ivlen);
        if(deriveKey_data->iv == NULL)
        {
            //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
            goto error;
        }
        memset(deriveKey_data->iv, 0, sizeof(unsigned char)*deriveKey_data->ivlen);
    }

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
