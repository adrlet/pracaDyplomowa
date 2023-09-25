#ifndef SYMMETRICDERIVEKEY_H_INCLUDED
#define SYMMETRICDERIVEKEY_H_INCLUDED

#include <gtk/gtk.h>

struct symmetric_derive_key;

gboolean symmetric_derive_key_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data);

gboolean symmetric_derive_key_password_close_cb(GtkDialog * passwordDialog, gpointer user_data);

void symmetric_derive_key_password_close2_cb(GtkWindow * passwordDialog, gpointer user_data);

void symmetric_derive_key_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data);

void symmetric_derive_key_proceed(struct symmetric_derive_key * derive_data);

#endif // SYMMETRICDERIVEKEY_H_INCLUDED
