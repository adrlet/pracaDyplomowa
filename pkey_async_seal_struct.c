#include <string.h>

#include "pkey_async_seal_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_SEAL_STRUCT_ADDRESS, type, code))

struct async_seal_data * async_seal_data_new()
{
    struct async_seal_data * data = NULL;

    data = (struct async_seal_data*)malloc(sizeof(struct async_seal_data));
    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    data->inputPath = NULL;
    data->outputPath = NULL;
    data->pubKeysPath = NULL;
    data->privKeyPath = NULL;
    data->ivPath = NULL;
    data->keyPath = NULL;

    data->passphrase = NULL;

    data->handler = NULL;

    data->keynum = 0;
    data->cipher = 0;
    data->privCipher = 0;

    data->flags = 0;

end:
    return data;
};

int async_seal_data_delete(struct async_seal_data * data)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    data->inputPath = NULL;
    data->outputPath = NULL;
    data->pubKeysPath = NULL;
    data->privKeyPath = NULL;
    data->ivPath = NULL;
    data->keyPath = NULL;

    data->passphrase = NULL;

    data->handler = NULL;

    data->keynum = 0;
    data->cipher = 0;
    data->privCipher = 0;

    data->flags = 0;

    free(data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_seal_data_provide(struct async_seal_data * data, const char * inputPath, const char * outputPath, const char ** pubKeysPath,
                            const char * privKeyPath, const char * ivPath, const char * keyPath, const char * passphrase,
                            struct asynchronicHandler * handler, int keynum, int cipher, int privCipher, unsigned char flags)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    data->inputPath = inputPath;
    data->outputPath = outputPath;
    data->pubKeysPath = pubKeysPath;
    data->privKeyPath = privKeyPath;
    data->ivPath = ivPath;
    data->keyPath = keyPath;

    data->passphrase = passphrase;

    data->handler = handler;

    data->keynum = keynum;
    data->cipher = cipher;
    data->privCipher = privCipher;

    data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
