#ifndef SYMMETRICENCRYPT_STRUCT_H_INCLUDED
#define SYMMETRICENCRYPT_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define SYMMETRIC_PANEL_OK 0x0
#define SYMMETRIC_PANEL_ERROR 0xFF
#define SYMMETRIC_PANEL_NO_INPUT 0x1
#define SYMMETRIC_PANEL_NO_OUTPUT 0x2
#define SYMMETRIC_PANEL_NO_ALGORITHM 0x3
#define SYMMETRIC_PANEL_NO_BLOCK_MODE 0x4
#define SYMMETRIC_PANEL_NO_KEY_SIZE 0x5

#define SYMMETRIC_MODE_ENCRYPT 0x0
#define SYMMETRIC_MODE_DECRYPT 0x1

#define BASE64 0x80
#define SALT 0x40

#define IN_USE 0x20

// Dane symetryczne
struct symmetric_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * progressBox; // 16
    GtkWidget * alertDialog; // 24

    GCancellable * cancel; // 32

    char * inputPath; // 40
    char * outputPath; // 48

    unsigned char * key; // 56
    unsigned char * iv; // 64

    int algorithm; // 68
    int blockmode; // 72
    int keysize; // 76

    int keylen; // 80
    int ivlen; // 84

    unsigned char flags; // 85
};

struct symmetric_data * symmetric_data_new();

void symmetric_data_delete(struct symmetric_data * data);

void symmetric_data_clear(void * user_data);

int symmetric_data_verify(struct symmetric_data * data);

void symmetric_data_provide_dialogs(struct symmetric_data * data, GtkWidget * progressBox, GtkWidget * alertDialog);

void symmetric_data_provide_window(struct symmetric_data * data, GtkWidget * mainWindow);

void symmetric_data_provide_data(void * user_data, unsigned char * key, int keylen, unsigned char * iv, int ivlen);

void symmetric_data_provide_flags(struct symmetric_data * data, unsigned char flags);

#endif // SYMMETRICENCRYPT_STRUCT_H_INCLUDED
