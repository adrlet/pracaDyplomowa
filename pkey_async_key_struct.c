#include <string.h>

#include "pkey_async_key_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_KEY_GEN_STRUCT_ADDRESS, type, code))

struct async_key_data * async_key_data_new()
{
    struct async_key_data * data = NULL;

    data = (struct async_key_data*)malloc(sizeof(struct async_key_data));
    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    data->passphrase = NULL;
    data->pubKeyPath = NULL;
    data->privKeyPath = NULL;
    data->paramPath = NULL;

    data->keySize = 0;
    data->cipherAlgorithm = 0;

    data->flags = 0;

end:
    return data;
};

int async_key_data_delete(struct async_key_data * data)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    data->passphrase = NULL;
    data->pubKeyPath = NULL;
    data->privKeyPath = NULL;
    data->paramPath = NULL;

    data->keySize = 0;
    data->cipherAlgorithm = 0;

    data->flags = 0;

    free(data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_key_data_provide(struct async_key_data * data, const char * passphrase, const char * pubKeyPath, const char * privKeyPath,
                           const char * paramPath, int keySize, int cipherAlgorithm, unsigned char flags)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    data->passphrase = passphrase;
    data->pubKeyPath = pubKeyPath;
    data->privKeyPath = privKeyPath;
    data->paramPath = paramPath;

    data->keySize = keySize;
    data->cipherAlgorithm = cipherAlgorithm;

    data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
