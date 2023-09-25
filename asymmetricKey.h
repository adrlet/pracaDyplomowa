#ifndef ASYMMETRICKEY_H_INCLUDED
#define ASYMMETRICKEY_H_INCLUDED

#include <gtk/gtk.h>

void asymmetric_key_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data);

gboolean asymmetric_key_password_close_cb(GtkDialog * passwordDialog, gpointer user_data);

void asymmetric_key_rsa_gen(GtkButton * rsaGenButton, gpointer user_data);

void asymmetric_key_on_param_gen(GtkButton * keyOnParamGenButton, gpointer user_data);

void asymmetric_key_check(GtkButton * keyCheckButton, gpointer user_data);

#endif // ASYMMETRICKEY_H_INCLUDED
