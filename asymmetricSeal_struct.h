#ifndef ASYMMETRICSEAL_STRUCT_H_INCLUDED
#define ASYMMETRICSEAL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define ASYMMETRIC_SEAL_SEAL 0x0
#define ASSYMETRIC_SEAL_OPEN 0x1

#define ASSYMETRIC_SEAL_MODE_MASK 0x1

#define IN_USE 0x20

#define ASSYMETRIC_SEAL_OK 0x0
#define ASSYMETRIC_SEAL_ERROR 0xFF
#define ASSYMETRIC_SEAL_NO_INPUT 0x1
#define ASSYMETRIC_SEAL_NO_OUTPUT 0x2
#define ASSYMETRIC_SEAL_NO_PUBKEY 0x3
#define ASSYMETRIC_SEAL_NO_PRIVKEY 0x4
#define ASSYMETRIC_SEAL_NO_IV 0x5
#define ASSYMETRIC_SEAL_NO_SESSION 0x6

#define SEAL_BASE64 0x80

struct asymmetric_seal_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * progressBox; // 16
    GtkWidget * alertDialog; // 24
    GtkWidget * passwordDialog; // 32

    const char * passphrase; // 40
    const char * inputPath; // 48
    const char * outputPath; // 56
    const char * pubKeyPath; // 64
    const char * privKeyPath; // 72
    const char * ivPath; // 80
    const char * keyPath; // 88

    GCancellable * cancel; // 96

    int keynum; // 100

    int algorithm; // 104
    int blockMode; // 108
    int keySize; // 112

    int privAlgorithm; // 116
    int privBlockMode; // 120
    int privKeySize; // 124

    unsigned char flags; // 125
};

struct asymmetric_seal_data * asymmetric_seal_data_new();

void asymmetric_seal_data_delete(struct asymmetric_seal_data * data);

void asymmetric_seal_data_clear(void * user_data);

int asymmetric_seal_data_verify(struct asymmetric_seal_data * data);

void asymmetric_seal_data_provide_dialogs(struct asymmetric_seal_data * data, GtkWidget * progressBox, GtkWidget * alertDialog,
                                          GtkWidget * passwordDialog);

void asymmetric_seal_data_provide_window(struct asymmetric_seal_data * data, GtkWidget * mainWindow);

void asymmetric_seal_data_provide_flags(struct asymmetric_seal_data * data, unsigned char flags);

void asymmetric_seal_data_provide_password(struct asymmetric_seal_data * data, const char * password);

#endif // ASYMMETRICSEAL_STRUCT_H_INCLUDED
