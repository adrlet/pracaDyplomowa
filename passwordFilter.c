#include <string.h>
#include "passwordFilter.h"

const char * specialChars = "!\"#$\'%&()*+,-./:;<=>\?@[\\]^`{|}~";

const char * hasLower(const char * text)
{
    const char * iterator = NULL;

    if(text == NULL)
        return NULL;

    for(iterator = text; iterator[0] != 0; iterator++)
        if(iterator[0] >= 'a' && iterator[0] <= 'z')
            return iterator;

    return NULL;
}

const char * hasUpper(const char * text)
{
    const char * iterator = NULL;

    if(text == NULL)
        return NULL;

    for(iterator = text; iterator[0] != 0; iterator++)
        if(iterator[0] >= 'A' && iterator[0] <= 'Z')
            return iterator;

    return NULL;
}

const char * hasNumber(const char * text)
{
    const char * iterator = NULL;

    if(text == NULL)
        return NULL;

    for(iterator = text; iterator[0] != 0; iterator++)
        if(iterator[0] >= '0' && iterator[0] <= '9')
            return iterator;

    return NULL;
}

int countUppers(const char * password)
{
    int count = 0;

    if(password == NULL)
        return count;

    while((password = hasUpper(password)) != NULL)
    {
        count++;
        password++;
    }

    return count;
}

int countLowers(const char * password)
{
    int count = 0;

    if(password == NULL)
        return count;

    while((password = hasLower(password)) != NULL)
    {
        count++;
        password++;
    }

    return count;
}

int countNumbers(const char * password)
{
    int count = 0;

    if(password == NULL)
        return count;

    while((password = hasNumber(password)) != NULL)
    {
        count++;
        password++;
    }

    return count;
}

int countSpecials(const char * password)
{
    int count = 0;
    const char * end = NULL;

    if(password == NULL)
        return count;

    end = password+strlen(password);

    while((password += strcspn(password, specialChars)) != end)
    {
        count++;
        password++;
    }

    return count;
}

int check_password(const char * password, const char * password_repeat)
{
    int passlen = 0;
    int pass_retrylen = 0;

    if(password == NULL || password[0] == 0)
        return PASS_IS_EMPTY;

    if(password_repeat == NULL || password_repeat[0] == 0)
        return PASS_RETRY_IS_EMPTY;

    passlen = strlen(password);
    if(passlen < PASS_MIN_LENGTH)
        return PASS_TOO_SHORT;
    if(passlen > PASS_MAX_LENGTH)
        return PASS_TOO_LONG;

    if(countSpecials(password) < PASS_MIN_SPECIALS)
        return PASS_DOESNT_CONTAIN_SPECIAL_CHAR;

    if(countLowers(password) < PASS_MIN_LOWERS)
        return PASS_DOESNT_CONTAIN_LOW_LETTER;

    if(countUppers(password) < PASS_MIN_UPPERS)
        return PASS_DOESNT_CONTAIN_UP_LETTER;

    if(countNumbers(password) < PASS_MIN_NUMBERS)
        return PASS_DOESNT_CONTAIN_NUMBER;

    pass_retrylen = strlen(password_repeat);
    if(passlen != pass_retrylen || strcmp(password, password_repeat) != 0)
        return PASS_AND_PASS_RETRY_ARE_DIFFERENT;

    return PASS_IS_OK;
}
