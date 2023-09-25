#ifndef ASYMMETRICSEALPANEL_STRUCT_H_INCLUDED
#define ASYMMETRICSEALPPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#include "asymmetricSeal_struct.h"

struct asymmetric_seal_wrapper
{
    GtkWidget * mainWindow; // 8

    GtkWidget * inputPathEntry; // 16
    GtkWidget * outputPathEntry; // 24
    GtkWidget * pubKeyPathEntry; // 32
    GtkWidget * privKeyPathEntry; // 40
    GtkWidget * ivPathEntry; // 48
    GtkWidget * keyPathEntry; // 56

    GtkWidget * algorithmDropDown; // 64
    GtkWidget * blockModeDropDown; // 72
    GtkWidget * keySizeDropDown; // 80

    GtkWidget * base64Check; // 88

    GtkWidget * privAlgorithmDropDown; // 96
    GtkWidget * privBlockModeDropDown; // 104
    GtkWidget * privKeySizeDropDown; // 112

    struct asymmetric_seal_data * data; // 120
};

struct asymmetric_seal_wrapper * asymmetric_seal_wrapper_new();

void asymmetric_seal_wrapper_delete(struct asymmetric_seal_wrapper * wrapper);

void asymmetric_seal_wrapper_provide_window(struct asymmetric_seal_wrapper * wrapper, GtkWidget * mainWindow);

void asymmetric_seal_wrapper_provide_widgets(struct asymmetric_seal_wrapper * wrapper, GtkWidget * inputPathEntry,
                                             GtkWidget * outputPathEntry, GtkWidget * pubKeyPathEntry, GtkWidget * privKeyPathEntry,
                                             GtkWidget * ivPathEntry, GtkWidget * keyPathEntry, GtkWidget * algorithmDropDown,
                                             GtkWidget * blockModeDropDown, GtkWidget * keySizeDropDown, GtkWidget * base64Check,
                                             GtkWidget * privAlgorithmDropDown, GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown);

void asymmetric_seal_wrapper_provide_data(struct asymmetric_seal_wrapper * wrapper, struct asymmetric_seal_data * data);

#endif // ASYMMETRICSEALPPANEL_STRUCT_H_INCLUDED
