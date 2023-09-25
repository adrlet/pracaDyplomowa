#ifndef ASYMMETRICPARAM_H_INCLUDED
#define ASYMMETRICPARAM_H_INCLUDED

#include <gtk/gtk.h>

gboolean asymmetric_param_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data);

void asymmetric_param_gen(GtkButton * paramGenButton, gpointer user_data);

void asymmetric_param_check(GtkButton * paramCheckButton, gpointer user_data);

#endif // ASYMMETRICPARAM_H_INCLUDED
