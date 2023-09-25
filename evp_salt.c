#include <string.h>

#include "evp_salt.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_SALT_ADDRESS, type, code))

const char * salted = "Salted__";

int writeSalt(BIO * output, const unsigned char * salt, int saltlen, unsigned char flush)
{
    int iResult = 0;
    int toWrite = 0;
    const void * buffer;

    if(output == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    if(salt == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(saltlen <= 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    toWrite = SALT_HEADER_LEN;
    buffer = salted;
    while(toWrite > 0)
    {
        iResult = BIO_write(output, buffer, toWrite);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 0);
            goto error;
        }
        buffer = (buffer+iResult*sizeof(char));
        toWrite = toWrite - iResult;
    }

    toWrite = saltlen;
    buffer = salt;
    while(toWrite > 0)
    {
        iResult = BIO_write(output, buffer, toWrite);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 1);
            goto error;
        }
        buffer = (buffer+iResult*sizeof(unsigned char));
        toWrite = toWrite - iResult;
    }

    if(flush == 1)
        BIO_flush(output);

    iResult = 0;
    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int readSalt(BIO * input, unsigned char * salt, int saltlen)
{
    int iResult = 0;
    int toRead = 0;
    char * header[SALT_HEADER_LEN];
    void * buffer;

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(salt == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(saltlen <= 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto error;
    }

    toRead = SALT_HEADER_LEN;
    buffer = header;
    while(BIO_eof(input) != 1 && toRead > 0)
    {
        iResult = BIO_read(input, buffer, toRead);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 2);
            goto error;
        }
        if(toRead == 0)
            continue;
        buffer = (buffer+iResult*sizeof(char));
        toRead = toRead - iResult;
    }

    if(memcmp(header, salted, SALT_HEADER_LEN) != 0)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 3);
        goto error;
    }

    toRead = saltlen;
    buffer = salt;
    while(BIO_eof(input) != 1 && toRead > 0)
    {
        iResult = BIO_read(input, buffer, toRead);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 4);
            goto error;
        }
        buffer = (buffer+iResult*sizeof(char));
        toRead = toRead - iResult;
    }

    iResult = 0;
    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
