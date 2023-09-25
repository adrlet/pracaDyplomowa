#ifndef PKEY_ENVELOPE_STRUCT_H_INCLUDED
#define PKEY_ENVELOPE_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_ENVELOPE_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(24)

struct envelopeData
{
    int key_num; // 4
    int block_len; // 8
    int iv_len; // 12
    EVP_PKEY ** pub_keys; // 24
    EVP_CIPHER * algorithm; // 32
    unsigned char ** encrypted_keys; // 40
    int * encrypted_key_len; // 48
    unsigned char * iv; // 56
};

struct envelopeData * setupEnvelope();

void freeEnvelope(struct envelopeData * envelopeParameters);

int provideEnvelopeAlgorithm(struct envelopeData * envelopeParameters, EVP_CIPHER * algorithm);

int provideEnvelopeKeys(struct envelopeData * envelopeParameters, EVP_PKEY ** keys, int count);

#endif // PKEY_ENVELOPE_STRUCT_H_INCLUDED
