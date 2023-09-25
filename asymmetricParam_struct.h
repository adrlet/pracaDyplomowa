#ifndef ASYMMETRICPARAM_STRUCT_H_INCLUDED
#define ASYMMETRICPARAM_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#define ASYMMETRIC_PARAM_GEN 0x0
#define ASSYMETRIC_PARAM_CHECK 0x1

#define ASSYMETRIC_PARAM_MODE_MASK 0x1

#define IN_USE 0x20

#define ASSYMETRIC_PARAM_OK 0x0
#define ASSYMETRIC_PARAM_ERROR 0xFF
#define ASSYMETRIC_PARAM_NO_PARAM 0x1

struct asymmetric_param_data
{
    GtkWidget * mainWindow; // 8
    GtkWidget * spinnerDialog; // 16
    GtkWidget * alertDialog; // 24

    const char * paramPath; // 56

    int keySize; // 68

    int algorithm; // 72

    unsigned char flags; // 81
};

struct asymmetric_param_data * asymmetric_param_data_new();

void asymmetric_param_data_delete(struct asymmetric_param_data * data);

void asymmetric_param_data_clear(void * user_data);

int asymmetric_param_data_verify(struct asymmetric_param_data * data);

void asymmetric_param_data_provide_dialogs(struct asymmetric_param_data * data, GtkWidget * spinnerDialog, GtkWidget * alertDialog);

void asymmetric_param_data_provide_window(struct asymmetric_param_data * data, GtkWidget * mainWindow);

void asymmetric_param_data_provide_flags(struct asymmetric_param_data * data, unsigned char flags);

void asymmetric_param_data_provide_password(struct asymmetric_param_data * data, const char * password);

#endif // ASYMMETRICPARAM_STRUCT_H_INCLUDED
