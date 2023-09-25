#ifndef ASYMMETRICAGREMEENT_H_INCLUDED
#define ASYMMETRICAGREMEENT_H_INCLUDED

#include <gtk/gtk.h>

gboolean asymmetric_agremeent_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data);

void asymmetric_agremeent_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data);

gboolean asymmetric_agremeent_password_close_cb(GtkDialog * passwordDialog, gpointer user_data);

void asymmetric_agremeent_gen(GtkButton * paramGenButton, gpointer user_data);

#endif // ASYMMETRICAGREMEENT_H_INCLUDED
