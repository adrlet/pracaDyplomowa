#ifndef SYMMETRICDERIVEKEY_STRUCT_H_INCLUDED
#define SYMMETRICDERIVEKEY_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

struct symmetric_derive_key
{
    GtkWidget * mainWindow; // 8
    GtkWidget * passwordDialog; // 16
    GtkWidget * spinnerDialog; // 24
    GtkWidget * alertDialog; // 32

    gpointer user_data; // 40
    void (*user_function)(void*); // 48
    void (*user_cleaner)(void*); // 56
    void (*transfer_data)(void*, unsigned char*, int, unsigned char*, int); // 64

    char * password; // 72
    unsigned char * key; // 80
    unsigned char * iv; // 88
    const char * saltpath; // 96

    int keylen; // 100
    int ivlen; // 104
    int saltlen; // 108
    int iterations; // 112

    unsigned char flags; // 113
};

struct symmetric_derive_key * symmetric_derive_key_new();

void symmetric_derive_key_delete(struct symmetric_derive_key * derive_key_data);

void symmetric_derive_key_clear(struct symmetric_derive_key * derive_key_data);

void symmetric_derive_key_provide_handlers(struct symmetric_derive_key * derive_key_data, gpointer user_data,
                                           void (*user_function)(void*), void (*user_cleaner)(void*),
                                           void (*transfer_data)(void*, unsigned char*, int, unsigned char*, int));

void symmetric_derive_key_provide_proceed_data(struct symmetric_derive_key * derive_key_data,
                                               unsigned char flags, const char * saltpath, int saltlen);

void symmetric_derive_key_provide_password(struct symmetric_derive_key * derive_key_data, const char * password);

void symmetric_derive_key_provide_dialogs(struct symmetric_derive_key * derive_key_data, GtkWidget * passwordDialog, GtkWidget * spinnerDialog, GtkWidget * alertDialog);

void symmetric_derive_key_provide_window(struct symmetric_derive_key * derive_key_data, GtkWidget * mainWindow);

void symmetric_derive_key_provide_data(struct symmetric_derive_key * derive_key_data, unsigned char * key, int keylen, unsigned char * iv, int ivlen, int iterations);

void symmetric_derive_key_provide_lengths(struct symmetric_derive_key * derive_key_data, int keylen, int ivlen);

#endif // SYMMETRICDERIVEKEY_STRUCT_H_INCLUDED
