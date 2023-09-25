#include <openssl/evp.h>
#include <openssl/bio.h>

#include "evp_async_encryptdecrypt.h"
#include "evp_async_encryptdecrypt_struct.h"
#include "evp_encryptdecrypt.h"
#include "evp_encryptdecrypt_struct.h"
#include "evp_salt.h"
#include "symmetricEncrypt_struct.h"

#include "thread_asynchronichandler.h"
#include "errorLog.h"
#include "bio_pre.h"
#include "bio_errorhandle.h"
#include "symmetric_cipher_map.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_ENCRYPT_DECRYPT_ADDRESS, type, code))

void * async_encrypt(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    int iResult = 0;
    int * iResult_alloc = NULL;
    unsigned char * salt = NULL;

    struct async_encrypt_data * encryptDecrypt_data = NULL;
    struct cipherData * cipherParams = NULL;
    BIO * input = NULL;
    BIO * output = NULL;

    encryptDecrypt_data = (struct async_encrypt_data*)arg;
    printf("async_encrypt\n");

    iResult_alloc = (int*)malloc(sizeof(int));
    if(iResult_alloc == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto error;
    }

    cipherParams = setupCipher();
    if(cipherParams == NULL)
        goto error;

    iResult = provideCipherAlgorithm(cipherParams, get_symmetric_algorithm(encryptDecrypt_data->cipher_algorithm_position));

    if(iResult == -1)
        goto error;

    iResult = provideCipherKeyIv(cipherParams, encryptDecrypt_data->key, encryptDecrypt_data->iv);
    if(iResult == -1)
        goto error;

    input = readFile(encryptDecrypt_data->inputPath);
    if(input == NULL)
        goto error;

    if(encryptDecrypt_data->flags & SALT)
    {
        if(encryptDecrypt_data->flags & BASE64)
        {
            salt = (unsigned char*)malloc(sizeof(unsigned char*)*(encryptDecrypt_data->saltlen));
            if(salt == NULL)
            {
                writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
                goto error;
            }
            output = readFile_Base64(encryptDecrypt_data->outputPath);
            if(output == NULL)
                goto error;
            iResult = readSalt(output, salt, encryptDecrypt_data->saltlen);
            if(iResult == -1)
                goto error;
            freeBio(output);
            output = NULL;

            output = writeFile_Base64(encryptDecrypt_data->outputPath);
            if(output == NULL)
                goto error;
            iResult = writeSalt(output, salt, encryptDecrypt_data->saltlen, 0);
            if(iResult == -1)
                goto error;
            free(salt);
            salt = NULL;
        }
        else
            output = appendFile(encryptDecrypt_data->outputPath);
        if(output == NULL)
            goto error;
    }
    else
    {
        if(encryptDecrypt_data->flags & BASE64)
            output = writeFile_Base64(encryptDecrypt_data->outputPath);
        else
            output = writeFile(encryptDecrypt_data->outputPath);
        if(output == NULL)
            goto error;
        }

    iResult = encrypt(input, output, cipherParams, encryptDecrypt_data->handler, encryptDecrypt_data->engine);
    if(iResult == -1)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    encryptDecrypt_data->handler->interrupt = 1;

    if(cipherParams != NULL)
        freeCipher(cipherParams);
    if(input != NULL)
        freeBio(input);
    if(output != NULL)
        freeBio(output);
    if(salt != NULL)
        free(salt);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_decrypt(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_encrypt_data * encryptDecrypt_data = NULL;
    struct cipherData * cipherParams = NULL;
    BIO * input = NULL;
    BIO * output = NULL;

    encryptDecrypt_data = (struct async_encrypt_data*)arg;
    printf("async_decrypt\n");

    iResult_alloc = (int*)malloc(sizeof(int));
    if(iResult_alloc == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 2);
        goto error;
    }

    cipherParams = setupCipher();
    if(cipherParams == NULL)
        goto error;

    iResult = provideCipherAlgorithm(cipherParams, get_symmetric_algorithm(encryptDecrypt_data->cipher_algorithm_position));
    if(iResult == -1)
        goto error;

    iResult = provideCipherKeyIv(cipherParams, encryptDecrypt_data->key, encryptDecrypt_data->iv);
    if(iResult == -1)
        goto error;

    if(encryptDecrypt_data->flags & BASE64)
        input = readFile_Base64(encryptDecrypt_data->inputPath);
    else
        input = readFile(encryptDecrypt_data->inputPath);
    if(input == NULL)
        goto error;

    if(encryptDecrypt_data->flags & SALT)
    {
        unsigned char symbole = 0;
        for(int i = 0; i < SALT_HEADER_LEN+encryptDecrypt_data->saltlen; i++)
        {
            iResult = BIO_read(input, &symbole, sizeof(unsigned char));
            if(iResult < 0)
            {
                writeErrorCode_full(ERROR_TYPE_BIO, 0);
                goto error;
            }
        }
    }

    output = writeFile(encryptDecrypt_data->outputPath);
    if(output == NULL)
        goto error;

    iResult = decrypt(input, output, cipherParams, encryptDecrypt_data->handler, encryptDecrypt_data->engine);
    if(iResult == -1)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    encryptDecrypt_data->handler->interrupt = 1;

    if(cipherParams != NULL)
        freeCipher(cipherParams);
    if(input != NULL)
        freeBio(input);
    if(output != NULL)
        freeBio(output);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
