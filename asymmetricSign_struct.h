#ifndef ASYMMETRICSIGN_STRUCT_H_INCLUDED
#define ASYMMETRICSIGN_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define ASYMMETRIC_SIGN_SIGN 0x0
#define ASSYMETRIC_SIGN_VERIFY 0x1

#define ASSYMETRIC_SIGN_MODE_MASK 0x1

#define IN_USE 0x20

#define ASSYMETRIC_SIGN_OK 0x0
#define ASSYMETRIC_SIGN_ERROR 0xFF
#define ASSYMETRIC_SIGN_NO_INPUT 0x1
#define ASSYMETRIC_SIGN_NO_OUTPUT 0x2
#define ASSYMETRIC_SIGN_NO_KEY_PATH 0x3

#define SIGN_BASE64 0x80

struct asymmetric_sign_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * progressBox; // 16
    GtkWidget * alertDialog; // 24
    GtkWidget * passwordDialog; // 32

    const char * passphrase; // 40
    const char * inputPath; // 48
    const char * outputPath; // 56
    const char * keyPath; // 64

    GCancellable * cancel; // 72

    int digest; // 76

    int privAlgorithm; // 84
    int privBlockMode; // 88
    int privKeySize; // 92

    unsigned char flags; // 93
};

struct asymmetric_sign_data * asymmetric_sign_data_new();

void asymmetric_sign_data_delete(struct asymmetric_sign_data * data);

void asymmetric_sign_data_clear(void * user_data);

int asymmetric_sign_data_verify(struct asymmetric_sign_data * data);

void asymmetric_sign_data_provide_dialogs(struct asymmetric_sign_data * data, GtkWidget * progressBox, GtkWidget * alertDialog,
                                          GtkWidget * passwordDialog);

void asymmetric_sign_data_provide_window(struct asymmetric_sign_data * data, GtkWidget * mainWindow);

void asymmetric_sign_data_provide_flags(struct asymmetric_sign_data * data, unsigned char flags);

void asymmetric_sign_data_provide_password(struct asymmetric_sign_data * data, const char * password);

#endif // ASYMMETRICSIGN_STRUCT_H_INCLUDED
