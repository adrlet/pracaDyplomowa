#ifndef SYMMETRICDIGESTPANEL_STRUCT_H_INCLUDED
#define SYMMETRICDIGESTPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

struct symmetric_digest_wrapper
{
    GtkWidget * mainWindow; // 8

    GtkWidget * inputEntry; // 16
    GtkWidget * outputEntry; // 24

    GtkWidget * algorithmDropDown; // 32

    GtkWidget * base64Check; // 56

    struct symmetric_digest_data * data; // 64
};

struct symmetric_digest_wrapper * symmetric_digest_wrapper_new();

void symmetric_digest_wrapper_delete(struct symmetric_digest_wrapper * wrapper);

void symmetric_digest_wrapper_provide_window(struct symmetric_digest_wrapper * wrapper, GtkWidget * mainWindow);

void symmetric_digest_wrapper_provide_widgets(struct symmetric_digest_wrapper * wrapper, GtkWidget * inputEntry, GtkWidget * outputEntry,
                                       GtkWidget * algorithmDropDown, GtkWidget * base64Check);

void symmetric_digest_wrapper_provide_data(struct symmetric_digest_wrapper * wrapper, struct symmetric_digest_data * data);

#endif // SYMMETRICDIGESTPANEL_STRUCT_H_INCLUDED
