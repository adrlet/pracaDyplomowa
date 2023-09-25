#include <stddef.h>

#include "evp_async_digest_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_DIGEST_STRUCT_ADDRESS, type, code))

struct async_digest_data * async_digest_data_new()
{
    struct async_digest_data * digest_data = NULL;

    digest_data = (struct async_digest_data*)malloc(sizeof(struct async_digest_data));
    if(digest_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    digest_data->inputPath = NULL;
    digest_data->outputPath = NULL;

    digest_data->engine = NULL;

    digest_data->handler = NULL;

    digest_data->digest_algorithm_position = 0;

    digest_data->flags = 0;

end:
    return digest_data;
};

int async_digest_data_delete(struct async_digest_data * digest_data)
{
    int iResult = 0;

    if(digest_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    digest_data->inputPath = NULL;
    digest_data->outputPath = NULL;

    digest_data->engine = NULL;

    digest_data->handler = NULL;

    digest_data->digest_algorithm_position = 0;

    digest_data->flags = 0;

    free(digest_data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_digest_data_provide(struct async_digest_data * digest_data, const char * inputPath, const char * outputPath,
                               ENGINE * engine, struct asynchronicHandler * handler, int digest_algorithm_position, unsigned char flags)
{
    int iResult = 0;

    if(digest_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    digest_data->inputPath = inputPath;
    digest_data->outputPath = outputPath;

    digest_data->engine = engine;

    digest_data->handler = handler;

    digest_data->digest_algorithm_position = digest_algorithm_position;

    digest_data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
