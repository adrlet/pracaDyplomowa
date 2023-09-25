#include <openssl/evp.h>

#include "evp_async_digest.h"
#include "evp_async_digest_struct.h"
#include "evp_digest.h"
#include "symmetricDigest_struct.h"

#include "bio_errorhandle.h"
#include "bio_pre.h"
#include "message_digest_map.h"
#include "errorLog.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_DIGEST_ADDRESS, type, code))

void * async_digest(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_digest_data * digest_data = NULL;

    BIO * input = NULL;
    BIO * output = NULL;
    const EVP_MD * digest_algorithm = NULL;

    digest_data = (struct async_digest_data*)arg;

    iResult_alloc = (int*)malloc(sizeof(int));
    if(iResult_alloc == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    input = readFile(digest_data->inputPath);
    if(input == NULL)
        goto error;

    if(digest_data->flags & BASE64)
        output = writeFile_Base64(digest_data->outputPath);
    else
        output = writeFile(digest_data->outputPath);
    if(output == NULL)
        goto error;

    digest_algorithm = get_digest_algorithm(digest_data->digest_algorithm_position);
    iResult = digest(input, output, digest_algorithm, digest_data->handler, NULL);
    if(iResult == -1)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    digest_data->handler->interrupt = 1;

    if(input != NULL)
        freeBio(input);
    if(output != NULL)
        freeBio(output);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
