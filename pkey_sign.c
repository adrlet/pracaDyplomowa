#include "pkey_sign.h"
#include "pkey_sign_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_SIGN_ADDRESS, type, code))

int sign(BIO * input, BIO * output, struct signData * signParameters, struct asynchronicHandler * handler, ENGINE * engine)
{
    int iResult;
    EVP_MD_CTX * mctx = NULL;
    unsigned char * inputBlock = NULL;
    unsigned char * outputBlock = NULL;
    int readenBytes;
    int sign_size;

    if(signParameters == NULL)
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


    mctx = EVP_MD_CTX_new();
    if(mctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto error;
    }

    inputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*signParameters->digest_size);
    if(inputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    iResult = EVP_DigestSignInit(mctx, NULL, signParameters->algorithm, engine, signParameters->key_pair);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
        goto error;
    }

    readenBytes = 1;
    while(BIO_eof(input) == 0 || readenBytes > 0)
    {
        readenBytes = BIO_read(input, inputBlock, signParameters->digest_size);
        if(readenBytes == 0)
            continue;
        if(readenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 0);
            goto error;
        }
        iResult = EVP_DigestSignUpdate(mctx, inputBlock, readenBytes);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
            goto error;
        }
        if(handler)
        {
            if(handler->interrupt == 1)
                goto error;
            handler->processed += readenBytes;
        }
    }
    OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    inputBlock = NULL;

    iResult = EVP_DigestSignFinal(mctx, NULL, (size_t*)&sign_size);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
        goto error;
    }

    outputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*sign_size);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
        goto error;
    }

    iResult = EVP_DigestSignFinal(mctx, outputBlock, (size_t*)&sign_size);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
        goto error;
    }

    iResult = BIO_write(output, outputBlock, sign_size);
    if(iResult < 0)
    {
        writeErrorCode_full(ERROR_TYPE_BIO, 1);
        goto error;
    }
    BIO_flush(output);

    iResult = 0;
    goto sign_end;

error:
    iResult = -1;

sign_end:
    if(mctx != NULL)
        EVP_MD_CTX_free(mctx);
    if(inputBlock != NULL)
        OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    if(outputBlock != NULL)
        OPENSSL_clear_free(outputBlock, sizeof(unsigned char)*sign_size);

    return iResult;
}

int hmacVerify(BIO * input, BIO * signInput, struct signData * signParameters, ENGINE * engine, int * result)
{
    int iResult;
    EVP_MD_CTX * mctx = NULL;
    unsigned char * comparedSign = NULL;
    unsigned char * inputBlock = NULL;
    unsigned char * outputBlock = NULL;
    int readenBytes;
    int sign_size;

    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    if(signInput == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 5);
        goto error;
    }

    mctx = EVP_MD_CTX_new();
    if(mctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 4);
        goto error;
    }

    inputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*signParameters->digest_size);
    if(inputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 3);
        goto error;
    }

    iResult = EVP_DigestSignInit(mctx, NULL, signParameters->algorithm, engine, signParameters->key_pair);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 5);
        goto error;
    }

    readenBytes = 1;
    while(BIO_eof(input) == 0 || readenBytes > 1)
    {
        readenBytes = BIO_read(input, inputBlock, signParameters->digest_size);
        if(readenBytes == 0)
            continue;
        if(readenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 2);
            goto error;
        }
        iResult = EVP_DigestSignUpdate(mctx, inputBlock, readenBytes);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 6);
            goto error;
        }
    }
    OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    inputBlock = NULL;

    iResult = EVP_DigestSignFinal(mctx, NULL, (size_t*)&sign_size);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 7);
        goto error;
    }

    outputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*sign_size);
    if(outputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 4);
        goto error;
    }

    iResult = EVP_DigestSignFinal(mctx, outputBlock, (size_t*)&sign_size);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 8);
        goto error;
    }

    comparedSign = (unsigned char*)OPENSSL_malloc(sizeof    (unsigned char)*sign_size);
    if(comparedSign == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 5);
        goto error;
    }

    readenBytes = 0;
    while(BIO_eof(signInput) == 0 && readenBytes < sign_size)
    {
        iResult = BIO_read(signInput, comparedSign+readenBytes, sign_size-readenBytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 3);
            goto error;
        }
        readenBytes += iResult;
    }

    *result = (readenBytes == sign_size) && (CRYPTO_memcmp(comparedSign, outputBlock, sign_size) == 0);
    iResult = 0;
    goto hmacVerify_end;

error:
    iResult = -1;

hmacVerify_end:
    if(mctx != NULL)
        EVP_MD_CTX_free(mctx);
    if(inputBlock != NULL)
        OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    if(outputBlock != NULL)
        OPENSSL_clear_free(outputBlock, sizeof(unsigned char)*sign_size);
    if(comparedSign != NULL)
        OPENSSL_clear_free(comparedSign, sizeof(unsigned char)*sign_size);

    return iResult;
}

int asymmetricVerify(BIO * input, BIO * signInput, struct signData * signParameters, struct asynchronicHandler * handler, ENGINE * engine, int * result)
{
    int iResult;
    EVP_MD_CTX * mctx = NULL;
    unsigned char * inputBlock = NULL;
    unsigned char * outputBlock = NULL;
    int readenBytes;
    int sign_size;

    if(signParameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 6);
        goto error;
    }

    if(input == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 7);
        goto error;
    }

    if(signInput == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 8);
        goto error;
    }

    mctx = EVP_MD_CTX_new();
    if(mctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 9);
        goto error;
    }

    inputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*signParameters->digest_size);
    if(inputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 6);
        goto error;
    }

    iResult = EVP_DigestVerifyInit(mctx, NULL, signParameters->algorithm, engine, signParameters->key_pair);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 10);
        goto error;
    }

    readenBytes = 1;
    while(BIO_eof(input) == 0 || readenBytes > 0)
    {
        readenBytes = BIO_read(input, inputBlock, signParameters->digest_size);
        if(readenBytes == 0)
            continue;
        if(readenBytes < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 4);
            goto error;
        }
        iResult = EVP_DigestVerifyUpdate(mctx, inputBlock, readenBytes);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 11);
            goto error;
        }
        if(handler)
        {
            if(handler->interrupt == 1)
                goto error;
            handler->processed += readenBytes;
        }
    }

    readenBytes = 0;
    while(BIO_eof(signInput) == 0 || iResult > 0)
    {
        iResult = BIO_read(signInput, inputBlock, signParameters->digest_size);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 5);
            goto error;
        }
        readenBytes += iResult;
    }
    sign_size = readenBytes;
    BIO_reset(signInput);
    BIO_seek(signInput, 0);

    OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    inputBlock = NULL;

    outputBlock = (unsigned char*)OPENSSL_malloc(sizeof(unsigned char)*sign_size);
    if(outputBlock == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 7);
        goto error;
    }

    readenBytes = 0;
    while(BIO_eof(signInput) == 0 && readenBytes < sign_size)
    {
        iResult = BIO_read(signInput, outputBlock+readenBytes, sign_size-readenBytes);
        if(iResult < 0)
        {
            writeErrorCode_full(ERROR_TYPE_BIO, 6);
            goto error;
        }
        readenBytes += iResult;
    }

    iResult = EVP_DigestVerifyFinal(mctx, outputBlock, sign_size);
    if(iResult < 0)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 12);
        goto error;
    }
    else if(iResult == 0)
        (*result) = 0;
    else
        (*result) = 1;

    goto asymmetricVerify_end;

error:
    iResult = -1;

asymmetricVerify_end:
    if(mctx != NULL)
        EVP_MD_CTX_free(mctx);
    if(inputBlock != NULL)
        OPENSSL_clear_free(inputBlock, sizeof(unsigned char)*signParameters->digest_size);
    if(outputBlock != NULL)
        OPENSSL_clear_free(outputBlock, sizeof(unsigned char)*sign_size);

    return iResult;
}
