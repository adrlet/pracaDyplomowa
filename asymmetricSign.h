#ifndef ASYMMETRICSIGN_H_INCLUDED
#define ASYMMETRICSIGN_H_INCLUDED

#include <gtk/gtk.h>

void _asymmetric_sign_progress_cancel_cb(GtkButton * progressButton, gpointer user_data);

void asymmetric_sign_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data);

gboolean asymmetric_sign_password_close_cb(GtkDialog * passwordDialog, gpointer user_data);

void asymmetric_sign_sign(GtkButton * signButton, gpointer user_data);

void asymmetric_sign_verify(GtkButton * verifyButton, gpointer user_data);

#endif // ASYMMETRICSIGN_H_INCLUDED
