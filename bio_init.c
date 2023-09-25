#include <openssl/bio.h>

#include "bio_init.h"
#include "bio_errorhandle.h"

BIO * standardInput = NULL;
BIO * standardOutput = NULL;

int initProcess()
{
    BIO ** _errorStack = getErrorStack();

    standardInput = BIO_new(BIO_s_file());
    standardOutput = BIO_new(BIO_s_file());
    (*_errorStack) = BIO_new(BIO_s_secmem());

    if(standardInput == NULL || standardOutput == NULL || (*_errorStack) == NULL)
        return -1;

    BIO_set_fp(standardInput, stdin, BIO_NOCLOSE);
    BIO_set_fp(standardOutput, stdout, BIO_NOCLOSE);
    BIO_set_close(*_errorStack, BIO_CLOSE);

    return 0;
}

void freeProcess()
{
    BIO ** _errorStack = getErrorStack();

    BIO_free(standardInput);
    BIO_free(standardOutput);
    BIO_free(*_errorStack);
}
