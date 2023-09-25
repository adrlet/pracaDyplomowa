#include "symmetricPanel.h"
#include "symmetricPanel_struct.h"
#include "symmetricDeriveKey.h"
#include "symmetricDeriveKey_struct.h"
#include "symmetricEncrypt.h"
#include "symmetricEncrypt_struct.h"

#include "widgetManagment.h"
#include "panelCallbacks.h"
#include "progressDialog.h"
#include "symmetric_cipher_map.h"

/// Sygnaly



/// Wczytanie panelu

struct symmetric_wrapper * symmetric_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "symmetricPanel.xml";

    // Uchwyty
    struct symmetric_wrapper * wrapper = NULL;
    struct symmetric_data * data = NULL;
    struct symmetric_derive_key * derive_key_data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * symInputEntry = NULL;
    GtkWidget * symInputButton = NULL;
    GtkWidget * symOutputEntry = NULL;
    GtkWidget * symOutputButton = NULL;

    // Dialogi
    GtkWidget * symInputFileChooserDialog = NULL;
    GtkWidget * symOutputFileChooserDialog = NULL;

    GtkWidget * passwordDialog = NULL;
    GtkWidget * spinnerDialog = NULL;
    GtkWidget * progressDialog = NULL;
    GtkWidget * alertDialog = NULL;

    GtkWidget * progressDialogContent = NULL;
    GtkWidget * progressBox = NULL;
    GtkWidget * progressButton = NULL;

    // Lista wysuwna
    GtkWidget * symAlgorithmDropDown = NULL;
    GtkWidget * symBlockModeDropDown = NULL;
    GtkWidget * symKeySizeDropDown = NULL;

    // Opcje
    GtkWidget * symBase64Check = NULL;
    GtkWidget * symSaltCheck = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogPassword.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);

    wrapper = symmetric_wrapper_new();
    data = symmetric_data_new();
    derive_key_data = symmetric_derive_key_new();

    wrapper->data = data;
    wrapper->derive_key_data = derive_key_data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    symmetric_wrapper_provide_window(wrapper, mainWindow);
    symmetric_data_provide_window(data, mainWindow);
    symmetric_derive_key_provide_window(derive_key_data, mainWindow);

    // Dialogi plików
    symInputFileChooserDialog = gtk_file_chooser_dialog_new("Wejście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(symInputFileChooserDialog, "close-request", G_CALLBACK(symInputFileChooserDialog_close_cb), NULL);

    symOutputFileChooserDialog = gtk_file_chooser_dialog_new("Wyjście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(symOutputFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    // Dialog hasła
    passwordDialog = GTK_WIDGET(gtk_builder_get_object(builder, "passwordDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(passwordDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(passwordDialog, "response", G_CALLBACK(symmetric_derive_key_cb), derive_key_data);
    g_signal_connect(passwordDialog, "close-request", G_CALLBACK(symmetric_derive_key_password_close_cb), derive_key_data);

    // Dialog spinner
    spinnerDialog = GTK_WIDGET(gtk_builder_get_object(builder, "spinnerDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(spinnerDialog), GTK_WINDOW(mainWindow));
    gtk_window_set_title(GTK_WINDOW(spinnerDialog), "Derywacja klucza");
    g_signal_connect(spinnerDialog, "close-request", G_CALLBACK(symmetric_derive_key_spiner_close_cb), derive_key_data);

    // Progress content
    progressDialog = progress_dialog_get_dialog();
    progressDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(progressDialog));

    progressBox = progress_dialog_get_template();
    progressButton = get_child_of_type(progressBox, GTK_TYPE_BUTTON, 0);
    g_signal_connect(progressButton, "clicked", G_CALLBACK(_symmetric_encode_progress_cancel_cb), data);
    gtk_box_append(GTK_BOX(progressDialogContent), progressBox);
    g_object_unref(progressBox);
    gtk_widget_set_visible(progressBox, FALSE);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    symInputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "symInputEntry"));
    symInputButton = GTK_WIDGET(gtk_builder_get_object(builder, "symInputButton"));
    symOutputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "symOutputEntry"));
    symOutputButton = GTK_WIDGET(gtk_builder_get_object(builder, "symOutputButton"));

    g_signal_connect(symInputFileChooserDialog, "response", G_CALLBACK(symInputFileChooserDialog_cb), symInputEntry);
    g_signal_connect(symInputButton, "clicked", G_CALLBACK(symInputButton_cb), symInputFileChooserDialog);
    g_signal_connect(symOutputFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), symOutputEntry);
    g_signal_connect(symOutputButton, "clicked", G_CALLBACK(symOutputButton_cb), symOutputFileChooserDialog);

    symAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "symAlgorithmDropDown"));
    symBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "symBlockModeDropDown"));
    symKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "symKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(symAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(symBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(symKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(symAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    symBase64Check = GTK_WIDGET(gtk_builder_get_object(builder, "symBase64Check"));
    symSaltCheck = GTK_WIDGET(gtk_builder_get_object(builder, "symSaltCheck"));

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    symmetric_wrapper_provide_widgets(wrapper, symInputEntry, symOutputEntry,
                                      symAlgorithmDropDown, symBlockModeDropDown, symKeySizeDropDown,
                                      symBase64Check, symSaltCheck);
    symmetric_data_provide_dialogs(data, progressBox, alertDialog);

    symmetric_derive_key_provide_dialogs(derive_key_data, passwordDialog, spinnerDialog, alertDialog);
    symmetric_derive_key_provide_handlers(derive_key_data, data, symmetric_encode_progress_init, symmetric_data_clear, symmetric_data_provide_data);

    return wrapper;
}
