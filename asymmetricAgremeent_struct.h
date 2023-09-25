#ifndef ASYMMETRICAGREMEENT_STRUCT_H_INCLUDED
#define ASYMMETRICAGREMEENT_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define IN_USE 0x20

#define ASSYMETRIC_AGREMEENT_OK 0x0
#define ASSYMETRIC_AGREMEENT_ERROR 0xFF
#define ASSYMETRIC_AGREMEENT_NO_KEY 0x1
#define ASSYMETRIC_AGREMEENT_NO_PUBKEY 0x2
#define ASSYMETRIC_AGREMEENT_NO_PRIVKEY 0x3
#define ASSYMETRIC_AGREMEENT_NO_OUTPUT 0x4

#define AGREMEENT_BASE64 0x80

struct asymmetric_agremeent_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * spinnerDialog; // 16
    GtkWidget * alertDialog; // 24
    GtkWidget * passwordDialog; // 32

    const char * passphrase; // 40
    const char * keyPath; // 32
    const char * privKeyPath;
    const char * pubKeyPath; // 40
    const char * outputPath; // 48

    int privAlgorithm; // 84
    int privBlockMode; // 88
    int privKeySize; // 92

    unsigned char flags; // 49
};

struct asymmetric_agremeent_data * asymmetric_agremeent_data_new();

void asymmetric_agremeent_data_delete(struct asymmetric_agremeent_data * data);

void asymmetric_agremeent_data_clear(void * user_data);

int asymmetric_agremeent_data_verify(struct asymmetric_agremeent_data * data);

void asymmetric_agremeent_data_provide_dialogs(struct asymmetric_agremeent_data * data, GtkWidget * spinnerDialog,
                                               GtkWidget * alertDialog, GtkWidget * passwordDialog);

void asymmetric_agremeent_data_provide_window(struct asymmetric_agremeent_data * data, GtkWidget * mainWindow);

void asymmetric_agremeent_data_provide_flags(struct asymmetric_agremeent_data * data, unsigned char flags);

void asymmetric_agremeent_data_provide_password(struct asymmetric_agremeent_data * data, const char * password);

#endif // ASYMMETRICSIGN_STRUCT_H_INCLUDED
