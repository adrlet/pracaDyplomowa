#include "bio_errorhandle_desc.h"

const char *** getDerive_keyString()
{
    static const char * input_null[] =
    {
        "pbkdf2_deriveKey:deriveParameters is Null",
        "pbkdf2_deriveKey:passphrase is Null",
        "pbkdf2_deriveKey:key is null",
        "pbkdf2_deriveKey:iv is null",
    };

    static const char * memory_allocation[] =
    {
        "pbkdf2_deriveKey:pbkdf2_out memory alloc failed"
    };

    static const char * bio[] = {};

    static const char * openssl[] =
    {
        "pbkdf2_deriveKey:PKCS5_PBKDF2_HMAC failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getDerive_key_structString()
{
    static const char * input_null[] =
    {
        "freeDerive:deriveParamters is NULL",
        "provideDeriveAlgorithm:deriveParamters is NULL",
        "provideDeriveAlgorithm:algorithm is NULL",
        "provideDeriveCipherInfo:deriveParameters is NULL",
        "provideDeriveCipherInfo:keylen and ivlen are NULL",
        "provideDeriveIterations:deriveParameters is NULL",
        "generateDeriveSalt:deriveParameters is NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupDerive:deriveParamters memory alloc failed",
        "generateDeriveSalt:salt memory alloc failed"
    };

    static const char * bio[] = {};

    static const char * openssl[] =
    {
        "generateDeriveSalt:RAND_bytes failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getEncryptdecryptString()
{
    static const char * input_null[] =
    {
        "encrypt:input is NULL",
        "encrypt:output is NULL",
        "encrypt:cipherParameters is NULL",
        "decrypt:input is NULL",
        "decrypt:output is NULL",
        "decrypt:cipherParameters is NULL"
    };

    static const char * memory_allocation[] =
    {
        "encrypt:input_block memory alloc failed",
        "encrypt:output_block memory alloc failed",
        "decrypt:input_block memory alloc failed",
        "decrypt:output_block memory alloc failed"
    };

    static const char * bio[] =
    {
        "encrypt:while BIO_read failed",
        "encrypt:while BIO_write failed",
        "encrypt:final BIO_write failed",
        "decrypt:while BIO_read failed",
        "decrypt:while BIO_write failed",
        "decrypt:final BIO_write failed"
    };

    static const char * openssl[] =
    {
        "encrypt:EVP_CIPHER_block_size failed",
        "encrypt:EVP_CIPHER_CTX_new failed",
        "encrypt:EVP_EncryptInit_ex failed",
        "encrypt:EVP_EncryptUpdate failed",
        "encrypt:EVP_EncryptFinal_ex failed",
        "decrypt:EVP_CIPHER_block_size failed",
        "decrypt:EVP_CIPHER_CTX_new failed",
        "decrypt:EVP_EncryptInit_ex failed",
        "decrypt:EVP_EncryptUpdate failed",
        "decrypt:EVP_EncryptFinal_ex failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getEncryptdecrypt_structString()
{
    static const char * input_null[] =
    {
        "freeCipher:cipherParameters is NULL",
        "provideCipherAlgorithm:cipherParameters is NULL",
        "provideCipherAlgorithm:algorithm is NULL",
        "provideCipherKeyIv:cipherParameters is NULL",
        "provideCipherKeyIv:key and iv are NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupCipher:cipherParameters memory alloc failed",
        "provideCipherKeyIv:key memory alloc failed",
        "provideCipherKeyIv:iv memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "provideCipherAlgorithm:EVP_CIPHER_key_length failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_encryptdecryptString()
{
    static const char * input_null[] =
    {
        "async_encrypt:arg is Null",
        "async_decrypt:arg is Null"
    };

    static const char * memory_allocation[] =
    {
        "async_encrypt:iResult memory alloc failed",
        "async_encrypt:salt memory alloc failed",
        "async_decrypt:iResult memory alloc failed"
    };

    static const char * bio[] =
    {
        "async_decrypt:salt BIO_read failed"
    };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_encryptdecrypt_structString()
{
    static const char * input_null[] =
    {
        "async_encrypt_data_delete:encrypt_data is Null",
        "async_encrypt_data_provide:encrypt_data is Null"
    };

    static const char * memory_allocation[] =
    {
        "async_encrypt_data_new:encrypt_data memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getThread_asynchronichandlerString()
{
    static const char * input_null[] = { };

    static const char * memory_allocation[] =
    {
        "setupAsynchronicHandler:handler memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getBio_preString()
{
    static const char * input_null[] =
    {
        "readFile:filename is Null",
        "writeFile:filename is Null",
        "appendFile:filename is Null"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] =
    {
        "readFile:BIO_new_file failed",
        "readFile:BIO_new(BIO_f_buffer()) failed",
        "writeFile:BIO_new_file failed",
        "writeFile:BIO_new(BIO_f_buffer()) failed",
        "appendFile:BIO_new_file failed",
        "appendFile:BIO_new(BIO_f_buffer()) failed",
        "readFile_Base64:BIO_new(BIO_f_base64()) failed",
        "writeFile_Base64:BIO_new(BIO_f_base64()) failed",
        "appendFile_Base64:BIO_new(BIO_f_base64()) failed"
    };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_derive_keyString()
{
    static const char * input_null[] =
    {
        "async_deriveKey:arg is Null"
    };

    static const char * memory_allocation[] =
    {
        "async_deriveKey:iResult_alloc memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_derive_key_structString()
{
    static const char * input_null[] =
    {
        "async_derivKey_data_delete:derivKey_data is Null",
        "async_derivKey_data_provide:derivKey_data is Null",
        "async_derivKey_data_init_buffers:derivKey_data is Null"
    };

    static const char * memory_allocation[] =
    {
        "async_derivKey_data_new:derivKey_data memory alloc failed",
        "async_derivKey_data_init_buffers:key memory alloc failed",
        "async_derivKey_data_init_buffers:iv memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getSaltString()
{
    static const char * input_null[] =
    {
        "write_salt:output is Null",
        "write_salt:salt is Null",
        "write_salt:saltlen is Null",
        "read_salt:input is Null",
        "read_salt:salt is Null",
        "read_salt:saltlen is Null"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] =
    {
        "write_salt:header BIO_write failed",
        "write_salt:salt BIO_write failed",
        "read_salt:header BIO_read failed",
        "read_salt:wrong header",
        "read_salt:salt BIO_read failed"
    };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getDigestString()
{
    static const char * input_null[] =
    {
        "digest:input is NULL",
        "digest:output is NULL",
        "digest:algorithm is NULL"
    };

    static const char * memory_allocation[] =
    {
        "digest:block memory alloc failed"
    };

    static const char * bio[] =
    {
        "digest:while BIO_read failed",
        "digest:while BIO_write failed"
    };

    static const char * openssl[] =
    {
        "digest:EVP_MD_block_size and EVP_MD_size failed",
        "digest:EVP_MD_CTX_new failed",
        "digest:EVP_DigestInit_ex failed",
        "digest:EVP_DigestUpdate failed",
        "digest:EVP_DigestFinal_ex failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_digestString()
{
    static const char * input_null[] =
    {
        "async_digest:arg is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_digest:iResult_alloc memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_digest_structString()
{
    static const char * input_null[] =
    {
        "async_digest_data_delete:digest_data is Null",
        "async_digest_data_provide:digest_data is Null"
    };

    static const char * memory_allocation[] =
    {
        "async_digest_data_new:digest_data memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getKey_genString()
{
    static const char * input_null[] =
    {
        "generateRsaKeys:count is NULL",
        "generateRsaKeys:bits is NULL",
        "generateRsaKeys:keys is NULL",
        "generateKeysOnParams:count is NULL",
        "generateKeysOnParams:parameters is NULL",
        "generateKeysOnParams:keys is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "generateRsaKeys:EVP_PKEY_CTX_new_id failed",
        "generateRsaKeys:EVP_PKEY_keygen_init failed",
        "generateRsaKeys:EVP_PKEY_CTX_set_rsa_keygen_bits failed",
        "generateRsaKeys:EVP_PKEY_keygen failed",
        "generateKeysOnParams:EVP_PKEY_CTX_new failed",
        "generateKeysOnParams:EVP_PKEY_keygen_init failed",
        "generateKeysOnParams:EVP_PKEY_keygen failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_key_genString()
{
    static const char * input_null[] =
    {
        "async_check:arg is NULL",
        "async_generate_rsa_key:arg is NULL",
        "async_generate_on_param_key:arg is NULL"
    };

    static const char * memory_allocation[] = {};

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_key_gen_structString()
{
    static const char * input_null[] =
    {
        "async_key_data_delete:data is NULL",
        "async_key_data_provide:data is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_key_data_new:data memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getParam_genString()
{
    static const char * input_null[] =
    {
        "generateParameters:count is NULL",
        "generateParameters:bit is NULL",
        "generateParameters:parameters is NULL",
        "generateParameters:type is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "generateParameters:EVP_PKEY_CTX_new_id failed",
        "generateParameters:EVP_PKEY_paramgen_init failed",
        "generateParameters:parameter_size_func failed",
        "generateParameters:EVP_PKEY_paramgen failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_param_genString()
{
    static const char * input_null[] =
    {
        "async_param_gen:arg is NULL",
        "async_param_check:arg is NULL"
    };

    static const char * memory_allocation[] = {};

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_param_gen_structString()
{
    static const char * input_null[] =
    {
        "async_param_data_delete:data is NULL",
        "async_param_data_provide:data is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_param_data_new:data memory alloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getCheckString()
{
    static const char * input_null[] =
    {
        "checkRsaKeyPair:key_pair is NULL",
        "checkDhPubKey:params is NULL",
        "checkDhPubKey:pub_key is NULL",
        "checkDhParams:params is NULL",
        "checkEccKeyPair:key_pair is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "checkRsaKeyPair:EVP_PKEY_get0_RSA failed",
        "checkRsaKeyPair:RSA_check_key failed",
        "checkDhPubKey:EVP_PKEY_get0_DH failed",
        "checkDhPubKey:EVP_PKEY_get0_DH failed",
        "checkDhPubKey:DH_get0_pub_key failed",
        "checkDhPubKey:DH_check_pub_key_ex failed",
        "checkDhParams:EVP_PKEY_get0_DH failed",
        "checkDhParams:DH_check_ex failed",
        "checkEccKeyPair:EVP_PKEY_get0_EC_KEY failed",
        "checkEccKeyPair:EC_KEY_check_key failed",
        "checkKey:EVP_PKEY_CTX_new failed",
        "checkKey:EVP_PKEY_check failed",
        "checkPubKey:EVP_PKEY_CTX_new failed",
        "checkPubKey:EVP_PKEY_public_check failed",
        "checkParams:EVP_PKEY_CTX_new failed",
        "checkParams:EVP_PKEY_param_check failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getKey_combineString()
{
    static const char * input_null[] =
    {
        "combineKeys:key_pair is NULL",
        "combineKeys:public_key is NULL",
        "combineKeys:pub_key is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] =
    {
        "combineKeys:BIO_new(BIO_s_secmem()) is NULL"
    };

    static const char * openssl[] =
    {
        "combineKeys:PEM_write_bio_PUBKEY failed",
        "combineKeys:PEM_read_bio_PUBKEY failed",
        "combineKeys:PEM_write_bio_PKCS8PrivateKey failed",
        "combineKeys:PEM_read_bio_PrivateKey failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getEnvelopeString()
{
    static const char * input_null[] =
    {
        "envelopeSeal:envelopeParameters is NULL",
        "envelopeSeal:input is NULL",
        "envelopeSeal:output is NULL",
        "envelopeOpen:envelopeParameters is NULL",
        "envelopeOpen:input is NULL",
        "envelopeOpen:output is NULL"
    };

    static const char * memory_allocation[] =
    {
        "envelopeSeal:inputBlock memory aloc failed",
        "envelopeSeal:outputBlock memory aloc failed",
        "envelopeOpen:inputBlock memory aloc failed",
        "envelopeOpen:outputBlock memory aloc failed"
    };

    static const char * bio[] =
    {
        "envelopeSeal:BIO_read failed",
        "envelopeSeal:BIO_write is NULL",
        "envelopeSeal:BIO_write is NULL",
        "envelopeOpen:BIO_read is NULL",
        "envelopeOpen:BIO_write is NULL",
        "envelopeOpen:BIO_write is NULL"
    };

    static const char * openssl[] =
    {
        "envelopeSeal:EVP_CIPHER_CTX_new failed",
        "envelopeSeal:EVP_SealInit failed",
        "envelopeSeal:EVP_SealUpdate failed",
        "envelopeSeal:EVP_SealFinal failed",
        "envelopeOpen:EVP_CIPHER_CTX_new failed",
        "envelopeOpen:EVP_OpenInit failed",
        "envelopeOpen:EVP_OpenUpdate failed",
        "envelopeOpen:EVP_OpenFinal failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getEnvelope_structString()
{
    static const char * input_null[] =
    {
        "freeEnvelope:envelopeParameters is NULL",
        "provideEnvelopeAlgorithm:envelopeParameters is NULL",
        "provideEnvelopeAlgorithm:algorithm is NULL",
        "provideEnvelopeKeys:envelopeParameters is NULL",
        "provideEnvelopeKeys:keys is NULL",
        "provideEnvelopeKeys:count is NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupEnvelope:envelopeParameters memory aloc failed",
        "provideEnvelopeAlgorithm:iv memory aloc failed",
        "provideEnvelopeKeys:pub_keys memory aloc failed",
        "provideEnvelopeKeys:encrypted_keys memory aloc failed",
        "provideEnvelopeKeys:encrypted_key_len memory aloc failed",
        "provideEnvelopeKeys:encrypted_keys memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "provideEnvelopeAlgorithm:EVP_CIPHER_block_size failed",
        "provideEnvelopeAlgorithm:EVP_CIPHER_iv_length failed",
        "provideEnvelopeKeys:EVP_PKEY_size failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_sealString()
{
    static const char * input_null[] =
    {
        "async_seal:arg is NULL",
        "async_open:arg is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_seal_structString()
{
    static const char * input_null[] =
    {
        "async_seal_data_delete:data is NULL",
        "async_seal_data_provide:data is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_seal_data_new:data memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getKey_agremeentString()
{
    static const char * input_null[] = { };

    static const char * memory_allocation[] =
    {
        "keyAgreement:secret memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "keyAgreement:EVP_PKEY_CTX_new failed",
        "keyAgreement:EVP_PKEY_derive_init failed",
        "keyAgreement:EVP_PKEY_derive_set_peer failed",
        "keyAgreement:EVP_PKEY_derive failed",
        "keyAgreement:EVP_PKEY_derive failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getKey_agremeent_structString()
{
    static const char * input_null[] =
    {
        "freeDh:dhParameters is NULL",
        "provideDhKeys:dhParameters is NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupDh:dhParameters memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_agremeentString()
{
    static const char * input_null[] =
    {
        "async_agremeent:arg is NULL",
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_agremeent_structString()
{
    static const char * input_null[] =
    {
        "async_agremeent_data_delete:data is NULL",
        "async_agremeent_data_provide:data is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_agremeent_data_new:data memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getParam_loadwriteString()
{
    static const char * input_null[] = { };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "extractParameters:PEM_write_bio_Parameters failed",
        "loadParameters:PEM_read_bio_Parameters failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getPriv_loadwriteString()
{
    static const char * input_null[] = { };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "extractRsaPrivateKey:PEM_write_bio_PKCS8PrivateKey failed",
        "importRsaPrivateKey:PEM_read_bio_PrivateKey failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getPriv_loadwrite_providerString()
{
    static const char * input_null[] =
    {
        "freeProvider:passwordProvider is NULL",
        "provideProviderPassword:passwordProvider is NULL",
        "provideProviderPassword:password and pass_provider are NULL",
        "provideProviderAlgorithm:passwordProvider is NULL",
        "provideProviderAlgorithm:algorithm is NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupProvider:passwordProvider memory aloc failed",
        "provideProviderPassword:passwordProvider memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getPub_loadwriteString()
{
    static const char * input_null[] = { };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "extractRsaPublicKey:PEM_write_bio_PUBKEY failed",
        "importRsaPublicKey:PEM_read_bio_PUBKEY failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getSignString()
{
    static const char * input_null[] =
    {
        "sign:signParameters is NULL",
        "sign:input is NULL",
        "sign:output is NULL",
        "hmacVerify:signParameters is NULL",
        "hmacVerify:input is NULL",
        "hmacVerify:signInput is NULL",
        "asymmetricVerify:signParameters are NULL",
        "asymmetricVerify:input is NULL",
        "asymmetricVerify:signInput is NULL"
    };

    static const char * memory_allocation[] =
    {
        "sign:inputBlock memory aloc failed",
        "sign:outputBlock memory aloc failed",
        "sign:outputBlock memory aloc failed",
        "hmacVerify:inputBlock memory aloc failed",
        "hmacVerify:outputBlock memory aloc failed",
        "hmacVerify:comparedSign memory aloc failed",
        "asymmetricVerify:inputBlock memory aloc failed",
        "asymmetricVerify:outputBlock memory aloc failed"
    };

    static const char * bio[] =
    {
        "sign:while BIO_read failed",
        "sign:final BIO_write failed",
        "hmacVerify:while BIO_write failed",
        "hmacVerify:final BIO_read failed",
        "asymmetricVerify:while BIO_read failed",
        "asymmetricVerify:while BIO_read failed",
        "asymmetricVerify:while BIO_read failed"
    };

    static const char * openssl[] =
    {
        "sign:EVP_MD_CTX_new failed",
        "sign:EVP_DigestSignInit failed",
        "sign:EVP_DigestSignUpdate failed",
        "sign:EVP_DigestSignFinal failed",
        "hmacVerify:EVP_MD_CTX_new failed",
        "hmacVerify:EVP_DigestSignInit failed",
        "hmacVerify:EVP_DigestSignUpdate failed",
        "hmacVerify:EVP_DigestSignFinal failed",
        "hmacVerify:EVP_DigestSignFinal failed",
        "asymmetricVerify:EVP_MD_CTX_new failed",
        "asymmetricVerify:EVP_DigestVerifyInit failed",
        "asymmetricVerify:EVP_DigestVerifyUpdate failed",
        "asymmetricVerify:EVP_DigestVerifyFinal failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getSign_structString()
{
    static const char * input_null[] =
    {
        "freeSign:signParameters is NULL",
        "provideSignDigest:signParameters is NULL",
        "provideSignDigest:algorithm is NULL",
        "provideSignKeys:signParameters is NULL",
        "provideSignKeys:pair is NULL"
    };

    static const char * memory_allocation[] =
    {
        "setupSign:signParameters memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] =
    {
        "provideSignDigest:EVP_MD_size or EVP_MD_block_size failed"
    };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_signString()
{
    static const char * input_null[] =
    {
        "async_sign:arg is NULL",
        "async_verify:arg is NULL"
    };

    static const char * memory_allocation[] = { };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}

const char *** getAsync_sign_structString()
{
    static const char * input_null[] =
    {
        "async_sign_data_delete:data is NULL",
        "async_sign_data_provide:data is NULL"
    };

    static const char * memory_allocation[] =
    {
        "async_sign_data_new:data memory aloc failed"
    };

    static const char * bio[] = { };

    static const char * openssl[] = { };

    static const char ** all[] =
    {
        input_null,
        memory_allocation,
        bio,
        openssl
    };

    return all;
}
