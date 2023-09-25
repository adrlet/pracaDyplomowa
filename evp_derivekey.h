#ifndef EVP_DERIVEKEY_H_INCLUDED
#define EVP_DERIVEKEY_H_INCLUDED

#include "evp_derivekey_struct.h"

#define ERROR_DERIVE_KEY_ADDRESS ERROR_FUNCTION_ADDRESS(1)

int pbkdf2_deriveKey(struct deriveData * deriveParameters, const char * passphrase, unsigned char * key, unsigned char * iv);

#endif // EVP_DERIVEKEY_H_INCLUDED
