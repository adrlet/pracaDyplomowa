#include "evp_encryptdecrypt.h"
#include "evp_encryptdecrypt_struct.h"

#include "thread_asynchronichandler.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ENCRYPT_DECRYPT_ADDRESS, type, code))

int encrypt(BIO * input, BIO * output, struct cipherData * cipherParameters, struct asynchronicHandler * handler, ENGINE * engine)
{
    int iResult;
    EVP_CIPHER_CTX * ctx = NULL;
    unsigned char * input_block = NULL;
    unsigned char * output_block = NULL;
    int readen_bytes;
    int written_bytes;
    int block_len;

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    if(output == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    block_len = EVP_CIPHER_block_size(cipherParameters->algorithm);
    if(block_len == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    input_block = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*block_len);
    if(input_block == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    output_block = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*block_len*4);
    if(output_block == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
        goto error;
    }

    iResult = EVP_EncryptInit_ex(ctx, cipherParameters->algorithm, engine, cipherParameters->key, cipherParameters->iv);
    if(iResult == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    readen_bytes = 1;
    while(BIO_eof(input) == 0 || readen_bytes > 0)
    {
        readen_bytes = BIO_read(input, input_block, block_len);
        if(readen_bytes == 0)
            continue;
        if(readen_bytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 0);
            goto error;
        }
        iResult = EVP_EncryptUpdate(ctx, output_block, &written_bytes, input_block, readen_bytes);
        if(iResult == 0)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
            goto error;
        }

        if(handler)
        {
            if(handler->interrupt == 1)
                goto error;
            handler->processed += readen_bytes;
        }
        iResult = BIO_write(output, output_block, written_bytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 1);
            goto error;
        }
    }

    iResult = EVP_EncryptFinal_ex(ctx, output_block, &written_bytes);
    if(iResult == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }
    if(written_bytes > 0)
    {
        iResult = BIO_write(output, output_block, written_bytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 2);
            goto error;
        }
    }
    BIO_flush(output);

    iResult = 0;
    goto encrypt_end;

error:
    iResult = -1;

encrypt_end:
    if(ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);
    if(input_block != NULL)
        OPENSSL_clear_free(input_block, sizeof(unsigned char)*block_len);
    if(output_block != NULL)
        OPENSSL_clear_free(output_block, sizeof(unsigned char)*block_len*2);

    return iResult;
}

int decrypt(BIO * input, BIO * output, struct cipherData * cipherParameters, struct asynchronicHandler * handler, ENGINE * engine)
{
    int iResult;
    EVP_CIPHER_CTX * ctx = NULL;
    unsigned char * input_block = NULL;
    unsigned char * output_block = NULL;
    int readen_bytes;
    int written_bytes;
    int block_size;

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(output == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(cipherParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto error;
    }

    block_size = EVP_CIPHER_block_size(cipherParameters->algorithm);
    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 5);
        goto error;
    }

    ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 6);
        goto error;
    }

    input_block = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*block_size);
    if(input_block == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 3);
        goto error;
    }

    output_block = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*block_size*2);
    if(output_block == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 4);
        goto error;
    }

    iResult = EVP_DecryptInit_ex(ctx, cipherParameters->algorithm, engine, cipherParameters->key, cipherParameters->iv);
    if(iResult == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 7);
        goto error;
    }

    readen_bytes = 1;
    while(BIO_eof(input) == 0 || readen_bytes > 0)
    {
        readen_bytes = BIO_read(input, input_block, block_size);
        if(readen_bytes == 0)
            continue;
        if(readen_bytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 3);
            goto error;
        }
        iResult = EVP_DecryptUpdate(ctx, output_block, &written_bytes, input_block, readen_bytes);
        if(iResult == 0)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 8);
            goto error;
        }
        if(handler)
        {
            if(handler->interrupt == 1)
                goto error;
            handler->processed += readen_bytes;
        }
        iResult = BIO_write(output, output_block, written_bytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 4);
            goto error;
        }
    }

    iResult = EVP_DecryptFinal_ex(ctx, output_block, &written_bytes);
    if(iResult == 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 9);
        goto error;
    }

    if(written_bytes > 0)
    {
        iResult = BIO_write(output, output_block, written_bytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 5);
            goto error;
        }
    }
    BIO_flush(output);

    iResult = 0;
    goto decrypt_end;

error:
    iResult = -1;

decrypt_end:
    if(ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);
    if(input_block != NULL)
        OPENSSL_clear_free(input_block, sizeof(unsigned char)*block_size);
    if(output_block != NULL)
        OPENSSL_clear_free(output_block, sizeof(unsigned char)*block_size*2);

    return iResult;
}
