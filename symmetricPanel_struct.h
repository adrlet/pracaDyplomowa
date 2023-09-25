#ifndef SYMMETRICPANEL_STRUCT_H_INCLUDED
#define SYMMETRICPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

// Dostep do widgetow panelu symetrycznego i owijka danych
struct symmetric_wrapper
{
    GtkWidget * mainWindow; // 8

    GtkWidget * inputEntry; // 16
    GtkWidget * outputEntry; // 24

    GtkWidget * algorithmDropDown; // 32
    GtkWidget * blockModeDropDown; // 40
    GtkWidget * keySizeDropDown; // 48

    GtkWidget * base64Check; // 56
    GtkWidget * saltCheck; // 64

    struct symmetric_data * data; // 80
    struct symmetric_derive_key * derive_key_data; // 88
};

struct symmetric_data;

// Inicjalizacja wrappera
struct symmetric_wrapper * symmetric_wrapper_new();

// Kasacja wrappera
void symmetric_wrapper_delete(struct symmetric_wrapper * wrapper);

// Wprowadzenie danych do struktury data z widgetow wewnatrz wrappera
void symmetric_wrapper_provide_window(struct symmetric_wrapper * wrapper, GtkWidget * mainWindow);

void symmetric_wrapper_provide_widgets(struct symmetric_wrapper * wrapper, GtkWidget * inputEntry, GtkWidget * outputEntry,
                                       GtkWidget * algorithmDropDown, GtkWidget * blockModeDropDown, GtkWidget * keySizeDropDown,
                                       GtkWidget * base64Check, GtkWidget * saltCheck);

void symmetric_wrapper_provide_data(struct symmetric_wrapper * wrapper, struct symmetric_data * data);

#endif // SYMMETRICPANEL_STRUCT_H_INCLUDED
