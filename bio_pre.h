#ifndef BIO_PRE_H_INCLUDED
#define BIO_PRE_H_INCLUDED

#include <openssl/bio.h>

#define ERROR_BIO_PRE_ADDRESS ERROR_FUNCTION_ADDRESS(10)

BIO * readFile(const char * filename);

BIO * writeFile(const char * filename);

BIO * appendFile(const char * filename);

BIO * readFile_Base64(const char * filename);

BIO * writeFile_Base64(const char * filename);

BIO * appendFile_Base64(const char * filename);

void freeBio(BIO * bio);

void setBufferSize(int size);

#endif // BIO_PRE_H_INCLUDED
