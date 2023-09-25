#ifndef PKEY_KEY_AGREMEENT_H_INCLUDED
#define PKEY_KEY_AGREMEENT_H_INCLUDED

#include <openssl/evp.h>

#include "pkey_key_agremeent_struct.h"

#define ERROR_KEY_AGREMEENT_ADDRESS ERROR_FUNCTION_ADDRESS(27)

int keyAgreement(struct dhData * dhParameters, ENGINE * engine);

#endif // PKEY_KEY_AGREMEENT_H_INCLUDED
