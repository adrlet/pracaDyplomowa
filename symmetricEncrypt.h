#ifndef SYMMETRICENCRYPT_H_INCLUDED
#define SYMMETRICENCRYPT_H_INCLUDED

#include <gtk/gtk.h>

void _symmetric_encode_progress_cancel2_cb(GtkButton * progressButton, gpointer user_data);

void _symmetric_encode_progress_cancel_cb(GtkButton * progressButton, gpointer user_data);

void symmetric_encode_progress_init(gpointer user_data);

void symmetric_decode_cb(GtkButton * decodeButton, gpointer user_data);

void symmetric_encode_cb(GtkButton * encodeButton, gpointer user_data);

#endif // SYMMETRICENCRYPT_H_INCLUDED
