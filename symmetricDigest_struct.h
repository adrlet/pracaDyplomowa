#ifndef SYMMETRICDIGEST_STRUCT_H_INCLUDED
#define SYMMETRICDIGEST_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define SYMMETRIC_DIGEST_OK 0x0
#define SYMMETRIC_DIGEST_ERROR 0xFF
#define SYMMETRIC_DIGEST_NO_INPUT 0x1
#define SYMMETRIC_DIGEST_NO_OUTPUT 0x2
#define SYMMETRIC_DIGEST_NO_ALGORITHM 0x3

#define BASE64 0x80
#define TO_DIALOG 0x40

#define IN_USE 0x20

struct symmetric_digest_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * progressBox; // 16
    GtkWidget * alertDialog; // 24

    GCancellable * cancel; // 32

    char * inputPath; // 40
    char * outputPath; // 48

    int algorithm; // 52

    unsigned char flags; // 53
};

struct symmetric_digest_data * symmetric_digest_data_new();

void symmetric_digest_data_delete(struct symmetric_digest_data * data);

void symmetric_digest_data_clear(void * user_data);

int symmetric_digest_data_verify(struct symmetric_digest_data * data);

void symmetric_digest_data_provide_dialogs(struct symmetric_digest_data * data, GtkWidget * progressBox, GtkWidget * alertDialog);

void symmetric_digest_data_provide_window(struct symmetric_digest_data * data, GtkWidget * mainWindow);

void symmetric_digest_data_provide_flags(struct symmetric_digest_data * data, unsigned char flags);

#endif // SYMMETRICDIGEST_STRUCT_H_INCLUDED
