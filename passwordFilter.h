#ifndef PASSWORDFILTER_H_INCLUDED
#define PASSWORDFILTER_H_INCLUDED

#define PASS_MIN_LENGTH 8
#define PASS_MAX_LENGTH 256
#define PASS_MIN_LOWERS 1
#define PASS_MIN_UPPERS 1
#define PASS_MIN_NUMBERS 1
#define PASS_MIN_SPECIALS 1

#define PASS_IS_OK 0x0
#define PASS_IS_EMPTY 0x1
#define PASS_RETRY_IS_EMPTY 0x2
#define PASS_TOO_SHORT 0x3
#define PASS_TOO_LONG 0x4
#define PASS_DOESNT_CONTAIN_SPECIAL_CHAR 0x5
#define PASS_DOESNT_CONTAIN_LOW_LETTER 0x6
#define PASS_DOESNT_CONTAIN_UP_LETTER 0x7
#define PASS_DOESNT_CONTAIN_NUMBER 0x8
#define PASS_AND_PASS_RETRY_ARE_DIFFERENT 0x9

const char * hasLower(const char * text);

const char * hasUpper(const char * text);

const char * hasUpper(const char * text);

int countUppers(const char * password);

int countLowers(const char * password);

int countNumbers(const char * password);

int countSpecials(const char * password);

int check_password(const char * password, const char * password_repeat);

#endif // PASSWORDFILTER_H_INCLUDED
