#ifndef ASYMMETRICKEY_STRUCT_H_INCLUDED
#define ASYMMETRICKEY_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define ASYMMETRIC_KEY_RSA_GEN 0x0
#define ASSYMETRIC_KEY_ON_PARAM_GEN 0x1
#define ASSYMETRIC_KEY_CHECK 0x2

#define ASSYMETRIC_KEY_MODE_MASK 0x3

#define IN_USE 0x20

#define ASSYMETRIC_KEY_OK 0x0
#define ASSYMETRIC_KEY_ERROR 0xFF
#define ASSYMETRIC_KEY_NO_KEY_PUB 0x1
#define ASSYMETRIC_KEY_NO_KEY_PRIV 0x2
#define ASSYMETRIC_KEY_NO_PARAM 0x3

struct asymmetric_key_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * spinnerDialog; // 16
    GtkWidget * alertDialog; // 24
    GtkWidget * passwordDialog; // 32

    const char * keyPubPath; // 40
    const char * keyPrivPath; // 48
    const char * paramPath; // 56

    const char * passphrase; // 64

    int keySize; // 68

    int privAlgorithm; // 72
    int privBlockMode; // 76
    int privKeySize; // 80

    unsigned char flags; // 81
};

struct asymmetric_key_data * asymmetric_key_data_new();

void asymmetric_key_data_delete(struct asymmetric_key_data * data);

void asymmetric_key_data_clear(void * user_data);

int asymmetric_key_data_verify(struct asymmetric_key_data * data, unsigned char flags);

void asymmetric_key_data_provide_dialogs(struct asymmetric_key_data * data, GtkWidget * spinnerDialog, GtkWidget * alertDialog,
                                         GtkWidget * passwordDialog);

void asymmetric_key_data_provide_window(struct asymmetric_key_data * data, GtkWidget * mainWindow);

void asymmetric_key_data_provide_flags(struct asymmetric_key_data * data, unsigned char flags);

void asymmetric_key_data_provide_password(struct asymmetric_key_data * data, const char * password);

#endif // ASYMMETRICKEY_STRUCT_H_INCLUDED
