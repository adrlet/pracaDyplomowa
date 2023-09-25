#ifndef SYMMETRICDIGEST_H_INCLUDED
#define SYMMETRICDIGEST_H_INCLUDED

#include <gtk/gtk.h>

void _symmetric_digest_progress_cancel_cb(GtkButton * progressButton, gpointer user_data);

void symmetric_digest_cb(GtkButton * digestButton, gpointer user_data);

#endif // SYMMETRICDIGEST_H_INCLUDED
