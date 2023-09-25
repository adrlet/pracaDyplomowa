#ifndef ASYMMETRICKEYPANEL_STRUCT_H_INCLUDED
#define ASYMMETRICKEYPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#include "asymmetricKey_struct.h"
#include "symmetricDeriveKey_struct.h"

struct asymmetric_key_wrapper
{
    GtkWidget * mainWindow;

    GtkWidget * keyPubInputEntry;
    GtkWidget * keyPrivInputEntry;
    GtkWidget * paramInputEntry;

    GtkWidget * keySizeEntry;

    GtkWidget * privAlgorithmDropDown;
    GtkWidget * privBlockModeDropDown;
    GtkWidget * privKeySizeDropDown;

    struct asymmetric_key_data * data;
};

struct asymmetric_key_wrapper * asymmetric_key_wrapper_new();

void asymmetric_key_wrapper_delete(struct asymmetric_key_wrapper * wrapper);

void asymmetric_key_wrapper_provide_window(struct asymmetric_key_wrapper * wrapper, GtkWidget * mainWindow);

void asymmetric_key_wrapper_provide_widgets(struct asymmetric_key_wrapper * wrapper, GtkWidget * keyPubInputEntry, GtkWidget * keyPrivInputEntry,
                                       GtkWidget * paramInputEntry, GtkWidget * keySizeEntry, GtkWidget * privAlgorithmDropDown,
                                       GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown);

void asymmetric_key_wrapper_provide_data(struct asymmetric_key_wrapper * wrapper, struct asymmetric_key_data * data);

#endif // ASYMMETRICKEYPANEL_STRUCT_H_INCLUDED
