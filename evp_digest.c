
#include "evp_digest.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_DIGEST_ADDRESS, type, code))

int digest(BIO * input, BIO * output, const EVP_MD * algorithm, struct asynchronicHandler * handler, ENGINE * engine)
{
    int iResult = 0;
    EVP_MD_CTX * ctx = NULL;
    unsigned char * block = NULL;
    int readen_bytes = 0;
    int written_bytes = 0;
    int block_size = 0;

    printf("%p\n", algorithm);

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

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    block_size = (EVP_MD_size(algorithm) > EVP_MD_block_size(algorithm) ? EVP_MD_size(algorithm) : EVP_MD_block_size(algorithm));
    if(block_size == -1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    block = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*block_size);
    if(block == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    ctx = EVP_MD_CTX_new();
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
        goto error;
    }

    iResult = EVP_DigestInit_ex(ctx, algorithm, engine);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    readen_bytes = 1;
    while(BIO_eof(input) == 0 || readen_bytes != 0)
    {
        readen_bytes = BIO_read(input, block, block_size);
        if(readen_bytes == 0)
            continue;
        if(readen_bytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 0);
            goto error;
        }
        iResult = EVP_DigestUpdate(ctx, block, readen_bytes);
        if(iResult != 1)
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
    }

    iResult = EVP_DigestFinal_ex(ctx, block, (unsigned int*)&written_bytes);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }
    written_bytes = BIO_write(output, block, written_bytes);
    if(written_bytes < 0)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 1);
        goto error;
    }
    BIO_flush(output);

    iResult = 0;
    goto digest_end;

error:
    iResult = -1;

digest_end:
    if(ctx != NULL)
        EVP_MD_CTX_free(ctx);
    if(block != NULL)
        OPENSSL_clear_free(block, sizeof(unsigned char)*block_size);

    return iResult;
}
