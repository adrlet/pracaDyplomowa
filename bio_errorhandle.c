#include <string.h>
#include <pthread.h>

#include "bio_errorhandle.h"
#include "bio_errorhandle_desc.h"

BIO * errorStack = NULL;

pthread_mutex_t	errorCode_mutex = PTHREAD_MUTEX_INITIALIZER;

int writeErrorCode(int code)
{
    int iResult;

    pthread_mutex_lock(&errorCode_mutex);
    iResult = BIO_write(errorStack, &code, sizeof(int));
    pthread_mutex_unlock(&errorCode_mutex);

    if(iResult < sizeof(int))
        return -1;

    return 0;
}

int getErrorCode()
{
    int iResult = 0;
    int retval = 0;

    pthread_mutex_lock(&errorCode_mutex);
    if((BIO_ctrl_pending(errorStack) & 0x3) != 0)
        BIO_read(errorStack, &retval, BIO_ctrl_pending(errorStack) & 0x3);

    if(BIO_eof(errorStack) != 0)
        goto error;

    iResult = BIO_read(errorStack, &retval, sizeof(int));
    if(iResult < sizeof(int))
        goto error;

    goto end;

error:
    retval = 0;
end:
    pthread_mutex_unlock(&errorCode_mutex);

    return retval;
}

static const char * getFunctionString(int code)
{
    static const int function_num = 14;
    static const char * functions[] =
    {
        "none", // 0
        "derive_key", // 1
        "derive_key_struct", // 2
        "async_derive_key", // 3
        "async_derive_key_struct", // 4
        "encrypt_decrypt", // 5
        "encrypt_decrypt_struct", // 6
        "async_encrypt_decrypt", // 7
        "async_encrypt_decrypt_struct", // 8
        "thread_asynchronic_handler", // 9
        "bio_pre", // 10
        "salt", // 11
        "digest", // 12
        "async_digest", // 13
        "async_digest_struct", // 14
        "key_gen", // 15
        "async_key_gen", // 16
        "async_key_gen_struct", // 17
        "param_gen", // 18
        "async_param_gen", // 19
        "async_param_gen_struct", // 20
        "check", // 21
        "key_combine", // 22
        "envelope", // 23
        "envelope_struct", // 24
        "async_seal", // 25
        "async_seal_struct", // 26
        "key_agremeent", // 27
        "key_agremeent_struct", // 28
        "async_agremeent", // 29
        "async_agremeent_struct", // 30
        "param_loadwrite", // 31
        "priv_loadwrite", // 32
        "priv_loadwrite_provider", // 33
        "pub_loadwrite", // 34
        "seal", // 35
        "seal_struct", // 36
        "async_struct", // 37
        "async_struct_sign" // 38
    };

    code = ((code & ERROR_FUNCTION_ADDRESS_MASK) >> ERROR_FUNCTION_ADDRESS_SPACE);

    if(code < 0 || code > function_num)
        return NULL;
    return functions[code];
}

static const char * getTypeString(int code)
{
    static const int type_num = 3;
    static const char * types[] =
    {
        "input_null", // 0
        "memory_allocation", // 1
        "bio", // 2
        "openssl", // 255
    };

    code = ((code & ERROR_TYPE_MASK) >> ERROR_TYPE_ADDRESS_SPACE);

    if(code == 255)
        return types[3];
    if(code < 0 || code > type_num)
        return NULL;
    return types[code];
}

static const char * getDescriptionString(int code)
{
    static const char *** function_pointer;
    static const char ** type_pointer;
    int function_code = (code & ERROR_FUNCTION_ADDRESS_MASK);
    int type_code = (code & ERROR_TYPE_MASK);
    int description_code = (code & ERROR_CODE_MASK);

    switch(function_code)
    {
    case ERROR_FUNCTION_ADDRESS(1):
        function_pointer = getDerive_keyString();
        break;
    case ERROR_FUNCTION_ADDRESS(2):
        function_pointer = getDerive_key_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(3):
        function_pointer = getAsync_derive_keyString();
        break;
    case ERROR_FUNCTION_ADDRESS(4):
        function_pointer = getAsync_derive_key_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(5):
        function_pointer = getEncryptdecryptString();
        break;
    case ERROR_FUNCTION_ADDRESS(6):
        function_pointer = getEncryptdecrypt_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(7):
        function_pointer = getAsync_encryptdecryptString();
        break;
    case ERROR_FUNCTION_ADDRESS(8):
        function_pointer = getAsync_encryptdecrypt_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(9):
        function_pointer = getThread_asynchronichandlerString();
        break;
    case ERROR_FUNCTION_ADDRESS(10):
        function_pointer = getBio_preString();
        break;
    case ERROR_FUNCTION_ADDRESS(11):
        function_pointer = getSaltString();
        break;
    case ERROR_FUNCTION_ADDRESS(12):
        function_pointer = getDigestString();
        break;
    case ERROR_FUNCTION_ADDRESS(13):
        function_pointer = getAsync_digestString();
        break;
    case ERROR_FUNCTION_ADDRESS(14):
        function_pointer = getAsync_digest_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(15):
        function_pointer = getKey_genString();
        break;
    case ERROR_FUNCTION_ADDRESS(16):
        function_pointer = getAsync_key_genString();
        break;
    case ERROR_FUNCTION_ADDRESS(17):
        function_pointer = getAsync_key_gen_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(18):
        function_pointer = getParam_genString();
        break;
    case ERROR_FUNCTION_ADDRESS(19):
        function_pointer = getAsync_param_genString();
        break;
    case ERROR_FUNCTION_ADDRESS(20):
        function_pointer = getAsync_param_gen_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(21):
        function_pointer = getCheckString();
        break;
    case ERROR_FUNCTION_ADDRESS(22):
        function_pointer = getKey_combineString();
        break;
    case ERROR_FUNCTION_ADDRESS(23):
        function_pointer = getEnvelopeString();
        break;
    case ERROR_FUNCTION_ADDRESS(24):
        function_pointer = getEnvelope_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(25):
        function_pointer = getAsync_sealString();
        break;
    case ERROR_FUNCTION_ADDRESS(26):
        function_pointer = getAsync_seal_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(27):
        function_pointer = getKey_agremeentString();
        break;
    case ERROR_FUNCTION_ADDRESS(28):
        function_pointer = getKey_agremeent_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(29):
        function_pointer = getAsync_agremeentString();
        break;
    case ERROR_FUNCTION_ADDRESS(30):
        function_pointer = getAsync_agremeent_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(31):
        function_pointer = getParam_loadwriteString();
        break;
    case ERROR_FUNCTION_ADDRESS(32):
        function_pointer = getParam_loadwriteString();
        break;
    case ERROR_FUNCTION_ADDRESS(33):
        function_pointer = getPriv_loadwrite_providerString();
        break;
    case ERROR_FUNCTION_ADDRESS(34):
        function_pointer = getPub_loadwriteString();
        break;
    case ERROR_FUNCTION_ADDRESS(35):
        function_pointer = getSignString();
        break;
    case ERROR_FUNCTION_ADDRESS(36):
        function_pointer = getSign_structString();
        break;
    case ERROR_FUNCTION_ADDRESS(37):
        function_pointer = getAsync_signString();
        break;
    case ERROR_FUNCTION_ADDRESS(38):
        function_pointer = getAsync_sign_structString();
        break;
    default:
        return NULL;
    }

    if(function_pointer == NULL)
        return NULL;

    switch(type_code)
    {
    case ERROR_TYPE_INPUT_NULL:
        type_pointer = function_pointer[0];
        break;
    case ERROR_TYPE_MEMORY_ALLOCATION:
        type_pointer = function_pointer[1];
        break;
    case ERROR_TYPE_BIO:
        type_pointer = function_pointer[2];
        break;
    case ERROR_TYPE_OPENSSL:
        type_pointer = function_pointer[3];
        break;
    default:
        return NULL;
    }

    if(type_pointer == NULL)
        return NULL;

    return type_pointer[description_code];
}

char * getErrorString(int code)
{
    const char * functionString = NULL;
    const char * typeString = NULL;
    const char * descriptionString = NULL;
    int totalLen = 0;
    char * output = NULL;

    totalLen = 4;
    functionString = getFunctionString(code);
    if(functionString != NULL)
        totalLen += strlen(functionString);

    typeString = getTypeString(code);
    if(typeString != NULL)
        totalLen += strlen(typeString);

    descriptionString = getDescriptionString(code);
    if(descriptionString != NULL)
        totalLen += strlen(descriptionString);

    output = (char*)malloc(sizeof(char)*(totalLen+1));
    if(output == NULL)
        goto end;
    output[0] = 0;

    strcat(output, ";");

    if(functionString != NULL)
        strcat(output, functionString);
    strcat(output, ";");

    if(typeString != NULL)
        strcat(output, typeString);
    strcat(output, ";");

    if(descriptionString != NULL)
        strcat(output, descriptionString);
    strcat(output, ";");

end:
    return output;
}

BIO ** getErrorStack()
{
    return &errorStack;
}
