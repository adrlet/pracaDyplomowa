#include "symmetricDigestPanel.h"
#include "symmetricDigestPanel_struct.h"
#include "symmetricDigest_struct.h"
#include "symmetricDigest.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "progressDialog.h"

/// Wczytanie panelu

struct symmetric_digest_wrapper * symmetric_digest_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "symmetricDigest.xml";

    // Uchwyty
    struct symmetric_digest_wrapper * wrapper = NULL;
    struct symmetric_digest_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * symDigestInputEntry = NULL;
    GtkWidget * symDigestInputButton = NULL;
    GtkWidget * symDigestOutputEntry = NULL;
    GtkWidget * symDigestOutputButton = NULL;

    // Dialogi
    GtkWidget * symDigestInputFileChooserDialog = NULL;
    GtkWidget * symDigestOutputFileChooserDialog = NULL;

    GtkWidget * progressDialog = NULL;
    GtkWidget * alertDialog = NULL;

    GtkWidget * progressDialogContent = NULL;
    GtkWidget * progressBox = NULL;
    GtkWidget * progressButton = NULL;

    // Lista wysuwna
    GtkWidget * symDigestAlgorithmDropDown = NULL;

    // Opcje
    GtkWidget * symDigestBase64Check = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogPassword.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);

    wrapper = symmetric_digest_wrapper_new();
    data = symmetric_digest_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    symmetric_digest_wrapper_provide_window(wrapper, mainWindow);
    symmetric_digest_data_provide_window(data, mainWindow);

    // Dialogi plików
    symDigestInputFileChooserDialog = gtk_file_chooser_dialog_new("Wejście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(symDigestInputFileChooserDialog, "close-request", G_CALLBACK(symInputFileChooserDialog_close_cb), NULL);

    symDigestOutputFileChooserDialog = gtk_file_chooser_dialog_new("Wyjście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(symDigestOutputFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    // Progress content
    progressDialog = progress_dialog_get_dialog();
    progressDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(progressDialog));

    progressBox = progress_dialog_get_template();
    progressButton = get_child_of_type(progressBox, GTK_TYPE_BUTTON, 0);
    g_signal_connect(progressButton, "clicked", G_CALLBACK(_symmetric_digest_progress_cancel_cb), data);
    gtk_box_append(GTK_BOX(progressDialogContent), progressBox);
    g_object_unref(progressBox);
    gtk_widget_set_visible(progressBox, FALSE);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    symDigestInputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestInputEntry"));
    symDigestInputButton = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestInputButton"));
    symDigestOutputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestOutputEntry"));
    symDigestOutputButton = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestOutputButton"));

    g_signal_connect(symDigestInputFileChooserDialog, "response", G_CALLBACK(symInputFileChooserDialog_cb), symDigestInputEntry);
    g_signal_connect(symDigestInputButton, "clicked", G_CALLBACK(symInputButton_cb), symDigestInputFileChooserDialog);
    g_signal_connect(symDigestOutputFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), symDigestOutputEntry);
    g_signal_connect(symDigestOutputButton, "clicked", G_CALLBACK(symOutputButton_cb), symDigestOutputFileChooserDialog);

    symDigestAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestAlgorithmDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(symDigestAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(settings_dialog_get_derive_algorithmValues())));

    symDigestBase64Check = GTK_WIDGET(gtk_builder_get_object(builder, "symDigestBase64Check"));

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    symmetric_digest_wrapper_provide_widgets(wrapper, symDigestInputEntry, symDigestOutputEntry,
                                      symDigestAlgorithmDropDown, symDigestBase64Check);
    symmetric_digest_data_provide_dialogs(data, progressBox, alertDialog);

    return wrapper;
}
