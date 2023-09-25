#include <openssl/evp.h>

#include "pkey_envelope.h"
#include "pkey_envelope_struct.h"

#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ENVELOPE_ADDRESS, type, code))

int envelopeSeal(BIO * input, struct envelopeData * envelopeParameters, BIO * output)
{
    EVP_CIPHER_CTX * ctx = NULL;
    int iResult;
    unsigned char * inputBlock = NULL;
    unsigned char * outputBlock = NULL;
    int readenBytes;
    int writtenBytes;

    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(output == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    inputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->block_len);
    if(inputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    outputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->block_len*2);
    if(outputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
        goto error;
    }

    iResult = EVP_SealInit(ctx, envelopeParameters->algorithm, envelopeParameters->encrypted_keys, envelopeParameters->encrypted_key_len,
                           envelopeParameters->iv, envelopeParameters->pub_keys, envelopeParameters->key_num);

    if(iResult < 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    readenBytes = 1;
    while(BIO_eof(input) == 0 || readenBytes > 0)
    {
        readenBytes = BIO_read(input, inputBlock, envelopeParameters->block_len);
        if(readenBytes == 0)
            continue;
        if(readenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 0);
            goto error;
        }
        iResult = EVP_SealUpdate(ctx, outputBlock, &writtenBytes, inputBlock, readenBytes);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
            goto error;
        }
        writtenBytes = BIO_write(output, outputBlock, writtenBytes);
        if(writtenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 1);
            goto error;
        }
    }
    iResult = EVP_SealFinal(ctx, outputBlock, &writtenBytes);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
        goto error;
    }
    if(writtenBytes > 0)
    {
        writtenBytes = BIO_write(output, outputBlock, writtenBytes);
        if(writtenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 2);
            goto error;
        }
    }
    BIO_flush(output);

    iResult = 0;
    goto envelopeSeal_end;

error:
    iResult = -1;

envelopeSeal_end:
    if(ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);
    if(inputBlock != NULL)
        OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*envelopeParameters->block_len);
    if(outputBlock != NULL)
        OPENSSL_clear_free(outputBlock, sizeof(unsigned char)*envelopeParameters->block_len*2);

    return iResult;
}

int envelopeOpen(BIO * input, struct envelopeData * envelopeParameters, BIO * output)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int iResult;
    unsigned char * inputBlock = NULL;
    unsigned char * outputBlock = NULL;
    int readenBytes;
    int writtenBytes;

    if(envelopeParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(output == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto error;
    }

    ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }

    inputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->block_len);
    if(inputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
        goto error;
    }

    outputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*envelopeParameters->block_len*2);
    if(outputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 3);
        goto error;
    }


    iResult = EVP_OpenInit(ctx, envelopeParameters->algorithm, (envelopeParameters->encrypted_keys)[0], (envelopeParameters->encrypted_key_len)[0],
                           envelopeParameters->iv, envelopeParameters->pub_keys[0]);
    if(iResult == 0)
    {

        writeErrorCode_full(ERROR_TYPE_OPENSSL, 5);
        goto error;
    }

    readenBytes = 1;
    while(BIO_eof(input) == 0 || readenBytes > 0)
    {
        readenBytes = BIO_read(input, inputBlock, envelopeParameters->block_len);
        if(readenBytes == 0)
            continue;
        if(readenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 3);
            goto error;
        }
        iResult = EVP_OpenUpdate(ctx, outputBlock, &writtenBytes, inputBlock, readenBytes);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 6);
            goto error;
        }
        writtenBytes = BIO_write(output, outputBlock, writtenBytes);
        if(writtenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 4);
            goto error;
        }
    }
    iResult = EVP_OpenFinal(ctx, outputBlock, &writtenBytes);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 7);
        goto error;
    }
    if(writtenBytes > 0)
    {
        writtenBytes = BIO_write(output, outputBlock, writtenBytes);
        if(writtenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 5);
            goto error;
        }
    }
    BIO_flush(output);

    iResult = 0;
    goto envelope_open_end;

error:
    iResult = -1;

envelope_open_end:
    if(ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);
    if(inputBlock != NULL)
        OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*envelopeParameters->block_len);
    if(outputBlock != NULL)
        OPENSSL_clear_free(outputBlock, sizeof(unsigned char)*envelopeParameters->block_len*2);

    return iResult;
}
