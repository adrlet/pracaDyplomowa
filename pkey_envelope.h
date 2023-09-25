#ifndef PKEY_ENVELOPE_H_INCLUDED
#define PKEY_ENVELOPE_H_INCLUDED

#include <openssl/bio.h>

#include "pkey_envelope_struct.h"

#define ERROR_ENVELOPE_ADDRESS ERROR_FUNCTION_ADDRESS(23)

int envelopeSeal(BIO * input, struct envelopeData * envelopeParameters, BIO * output);

int envelopeOpen(BIO * input, struct envelopeData * envelopeParameters, BIO * output);

#endif // PKEY_ENVELOPE_H_INCLUDED
