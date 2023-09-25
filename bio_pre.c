#include <openssl/evp.h>

#include "bio_pre.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_BIO_PRE_ADDRESS, type, code))

int buffer_size = 0;

BIO * readFile(const char * filename)
{
    BIO * file_bio = NULL;
    BIO * buffer = NULL;
    BIO * retval;
    static const char mode[] = "rb";

    if(filename == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    file_bio = BIO_new_file(filename, mode);
    if(file_bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 0);
        goto error;
    }

    buffer = BIO_new(BIO_f_buffer());
    if(buffer == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 1);
        goto error;
    }
    BIO_set_buffer_size(buffer, buffer_size);

    retval = BIO_push(buffer, file_bio);
    goto readFile_end;

error:
    if(buffer != NULL)
        BIO_free(buffer);
    if(file_bio != NULL)
        BIO_free(file_bio);

    retval = NULL;

readFile_end:
    return retval;
}

BIO * writeFile(const char * filename)
{
    BIO * file_bio = NULL;
    BIO * buffer = NULL;
    BIO * retval;
    static const char mode[] = "wb";

    if(filename == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    file_bio = BIO_new_file(filename, mode);
    if(file_bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 2);
        goto error;
    }

    buffer = BIO_new(BIO_f_buffer());
    if(buffer == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 3);
        goto error;
    }
    BIO_set_buffer_size(buffer, buffer_size);

    retval = BIO_push(buffer, file_bio);
    goto writeFile_end;

error:
    if(file_bio != NULL)
        BIO_free(file_bio);
    if(buffer != NULL)
        BIO_free(buffer);

    retval = NULL;

writeFile_end:
    return retval;
}

BIO * appendFile(const char * filename)
{
    BIO * file_bio = NULL;
    BIO * buffer = NULL;
    BIO * retval;
    static const char mode[] = "ab";

    if(filename == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    file_bio = BIO_new_file(filename, mode);
    if(file_bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 4);
        goto error;
    }

    buffer = BIO_new(BIO_f_buffer());
    if(buffer == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 5);
        goto error;
    }
    BIO_set_buffer_size(buffer, buffer_size);

    retval = BIO_push(buffer, file_bio);
    goto appendFile_end;

error:
    if(file_bio != NULL)
        BIO_free(file_bio);
    if(buffer != NULL)
        BIO_free(buffer);

    retval = NULL;

appendFile_end:
    return retval;
}

BIO * readFile_Base64(const char * filename)
{
    BIO * fileBio = NULL;
    BIO * base64Bio = NULL;
    BIO * retval = NULL;

    fileBio = readFile(filename);
    if(fileBio == NULL)
        goto readFile_Base64_end;

    base64Bio = BIO_new(BIO_f_base64());
    if(base64Bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 6);
        goto error;
    }
    BIO_set_flags(base64Bio, BIO_FLAGS_BASE64_NO_NL);

    retval = BIO_push(base64Bio, fileBio);
    goto readFile_Base64_end;

error:
    if(base64Bio != NULL)
        BIO_free(base64Bio);
    if(fileBio != NULL)
        freeBio(fileBio);

    retval = NULL;

readFile_Base64_end:
    return retval;
}

BIO * writeFile_Base64(const char * filename)
{
    BIO * fileBio = NULL;
    BIO * base64Bio = NULL;
    BIO * retval = NULL;

    fileBio = writeFile(filename);
    if(fileBio == NULL)
        goto writeFile_Base64_end;

    base64Bio = BIO_new(BIO_f_base64());
    if(base64Bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 7);
        goto error;
    }
    BIO_set_flags(base64Bio, BIO_FLAGS_BASE64_NO_NL);

    retval = BIO_push(base64Bio, fileBio);
    goto writeFile_Base64_end;

error:
    if(base64Bio != NULL)
        BIO_free(base64Bio);
    if(fileBio != NULL)
        freeBio(fileBio);

    retval = NULL;

writeFile_Base64_end:
    return retval;
}

BIO * appendFile_Base64(const char * filename)
{
    BIO * fileBio = NULL;
    BIO * base64Bio = NULL;
    BIO * retval = NULL;

    fileBio = appendFile(filename);
    if(fileBio == NULL)
        goto appendFile_Base64_end;

    base64Bio = BIO_new(BIO_f_base64());
    if(base64Bio == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 8);
        goto error;
    }
    BIO_set_flags(base64Bio, BIO_FLAGS_BASE64_NO_NL);

    retval = BIO_push(base64Bio, fileBio);
    goto appendFile_Base64_end;

error:
    if(base64Bio != NULL)
        BIO_free(base64Bio);
    if(fileBio != NULL)
        freeBio(fileBio);

    retval = NULL;

appendFile_Base64_end:
    return retval;
}

void freeBio(BIO * bio)
{
    BIO * next;

    next = BIO_pop(bio);
    while(bio != NULL)
    {
        BIO_free(bio);
        bio = next;
        next = BIO_pop(bio);
    }
}

void setBufferSize(int size)
{
    buffer_size = size;
}
