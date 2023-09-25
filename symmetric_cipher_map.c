#include "symmetric_cipher_map.h"

static const EVP_CIPHER * get_aes_128(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_aes_128_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_aes_128_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_aes_128_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_aes_128_ctr();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_aes_192(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_aes_192_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_aes_192_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_aes_192_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_aes_192_ctr();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_aes_256(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_aes_256_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_aes_256_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_aes_256_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_aes_256_ctr();
    default:
        return NULL;
    }
}


static const EVP_CIPHER * get_aes(int code)
{
    int keysize = (code & CIPHER_KEY_SIZE_ADDRESS_MASK);

    switch(keysize)
    {
    case CIPHER_KEY_SIZE_128:
        return get_aes_128(code);
    case CIPHER_KEY_SIZE_192:
        return get_aes_192(code);
    case CIPHER_KEY_SIZE_256:
        return get_aes_256(code);
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_blowfish_static(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_bf_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_bf_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_bf_ofb();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_blowfish(int code)
{
    int keysize = (code & CIPHER_KEY_SIZE_ADDRESS_MASK);

    switch(keysize)
    {
    case CIPHER_KEY_SIZE_STATIC:
        return get_blowfish_static(code);
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_camellia_128(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_camellia_128_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_camellia_128_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_camellia_128_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_camellia_128_ctr();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_camellia_192(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_camellia_192_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_camellia_192_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_camellia_192_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_camellia_192_ctr();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_camellia_256(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_camellia_256_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_camellia_256_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_camellia_256_ofb();
    case CIPHER_BLOCK_MODE_CTR:
        return EVP_camellia_256_ctr();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_camellia(int code)
{
    int keysize = (code & CIPHER_KEY_SIZE_ADDRESS_MASK);

    switch(keysize)
    {
    case CIPHER_KEY_SIZE_128:
        return get_camellia_128(code);
    case CIPHER_KEY_SIZE_192:
        return get_camellia_192(code);
    case CIPHER_KEY_SIZE_256:
        return get_camellia_256(code);
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_idea_static(int code)
{
    int blockmode = (code & CIPHER_BLOCK_MODE_ADDRESS_MASK);

    switch(blockmode)
    {
    case CIPHER_BLOCK_MODE_CBC:
        return EVP_idea_cbc();
    case CIPHER_BLOCK_MODE_CFB:
        return EVP_idea_cfb();
    case CIPHER_BLOCK_MODE_OFB:
        return EVP_idea_ofb();
    default:
        return NULL;
    }
}

static const EVP_CIPHER * get_idea(int code)
{
    int keysize = (code & CIPHER_KEY_SIZE_ADDRESS_MASK);

    switch(keysize)
    {
    case CIPHER_KEY_SIZE_STATIC:
        return get_idea_static(code);
    default:
        return NULL;
    }
}

const EVP_CIPHER * get_symmetric_algorithm(int code)
{
    int algorithm = (code & CIPHER_ALGORITHM_ADDRESS_MASK);

    switch(algorithm)
    {
    case CIPHER_ALGORITHM_AES:
        return get_aes(code);
    case CIPHER_ALGORITHM_BLOWFISH:
        return get_blowfish(code);
    case CIPHER_ALGORITHM_CAMELLIA:
        return get_camellia(code);
    case CIPHER_ALGORITHM_IDEA:
        return get_idea(code);
    default:
        return NULL;
    }
}

const char * const algorithms[] = { "AES", "Blowfish", "Camellia", "IDEA", NULL };

const char * const blockModes1[] = { "CBC", "CFB", "OFB", "CTR", NULL };
const char * const blockModes2[] = { "CBC", "CFB", "OFB", NULL };
const char * const blockModes3[] = { "CBC", "CFB", "OFB", "CTR", NULL };
const char * const blockModes4[] = { "CBC", "CFB", "OFB", NULL };

const char * const * const blockModes[] = { blockModes1, blockModes2, blockModes3, blockModes4 };

const char * const keySizes1[] = { "128", "196", "256", NULL };
const char * const keySizes2[] = { "128", NULL };
const char * const keySizes3[] = { "128", "196", "256", NULL };
const char * const keySizes4[] = { "128", NULL };

const char * const * const keySizes[] = { keySizes1, keySizes2, keySizes3, keySizes4 };

const char ** const symmetric_cipher_get_algorithms()
{
    return algorithms;
}

const char *** const symmetric_cipher_get_blockmodes()
{
    return blockModes;
}

const char *** const symmetric_cipher_get_keysize()
{
    return keySizes;
}
