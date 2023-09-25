#ifndef ASYMMETRICSEAL_H_INCLUDED
#define ASYMMETRICSEAL_H_INCLUDED

#include <gtk/gtk.h>

void _asymmetric_seal_progress_cancel_cb(GtkButton * progressButton, gpointer user_data);

void asymmetric_seal_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data);

gboolean asymmetric_seal_password_close_cb(GtkDialog * passwordDialog, gpointer user_data);

void asymmetric_seal_seal(GtkButton * sealButton, gpointer user_data);

void asymmetric_seal_open(GtkButton * verifyButton, gpointer user_data);

#endif // ASYMMETRICSEAL_H_INCLUDED
