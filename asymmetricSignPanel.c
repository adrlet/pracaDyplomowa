#include "asymmetricSignPanel.h"
#include "asymmetricSignPanel_struct.h"
#include "asymmetricSign.h"
#include "asymmetricSign_struct.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "message_digest_map.h"
#include "asymmetric_algorithm_map.h"
#include "symmetric_cipher_map.h"
#include "progressDialog.h"

/// Wczytanie panelu

struct asymmetric_sign_wrapper * asymmetric_sign_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "asymmetricSign.xml";

    // Uchwyty
    struct asymmetric_sign_wrapper * wrapper = NULL;
    struct asymmetric_sign_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * asymSignInputEntry = NULL;
    GtkWidget * asymSignInputButton = NULL;
    GtkWidget * asymSignOutputEntry = NULL;
    GtkWidget * asymSignOutputButton = NULL;
    GtkWidget * asymSignKeyEntry = NULL;
    GtkWidget * asymSignKeyButton = NULL;

    // Dialogi
    GtkWidget * asymSignInputChooserDialog = NULL;
    GtkWidget * asymSignOutputFileChooserDialog = NULL;
    GtkWidget * asymSignKeyChooserDialog = NULL;
    GFile * curFolder = NULL;

    GtkWidget * progressDialog = NULL;
    GtkWidget * alertDialog = NULL;
    GtkWidget * passwordDialog = NULL;

    GtkWidget * progressDialogContent = NULL;
    GtkWidget * progressBox = NULL;
    GtkWidget * progressButton = NULL;

    // Inne dane
    GtkWidget * asymSignDigestDropDown = NULL;
    GtkWidget * asymBase64Check = NULL;

    GtkWidget * asymKeyPrivAlgorithmDropDown = NULL;
    GtkWidget * asymKeyPrivBlockModeDropDown = NULL;
    GtkWidget * asymKeyPrivKeySizeDropDown = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogPassword.xml", NULL);

    wrapper = asymmetric_sign_wrapper_new();
    data = asymmetric_sign_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    asymmetric_sign_wrapper_provide_window(wrapper, mainWindow);
    asymmetric_sign_data_provide_window(data, mainWindow);

    // Dialogi plików
    asymSignInputChooserDialog = gtk_file_chooser_dialog_new("Wejście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSignInputChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSignOutputFileChooserDialog = gtk_file_chooser_dialog_new("Podpis", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSignOutputFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSignKeyChooserDialog = gtk_file_chooser_dialog_new("Klucz publiczny/prywatny", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSignKeyChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    curFolder = g_file_new_for_path(settings_get_settings()->keysPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymSignKeyChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);

    // Dialog progress
    progressDialog = progress_dialog_get_dialog();
    progressDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(progressDialog));

    progressBox = progress_dialog_get_template();
    progressButton = get_child_of_type(progressBox, GTK_TYPE_BUTTON, 0);
    g_signal_connect(progressButton, "clicked", G_CALLBACK(_asymmetric_sign_progress_cancel_cb), data);
    gtk_box_append(GTK_BOX(progressDialogContent), progressBox);
    g_object_unref(progressBox);
    gtk_widget_set_visible(progressBox, FALSE);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Dialog Password
    passwordDialog = GTK_WIDGET(gtk_builder_get_object(builder, "passwordDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(passwordDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(passwordDialog, "response", G_CALLBACK(asymmetric_sign_password_cb), data);
    g_signal_connect(passwordDialog, "close-request", G_CALLBACK(asymmetric_sign_password_close_cb), data);

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    asymSignInputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignInputEntry"));
    asymSignInputButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignInputButton"));
    asymSignOutputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignOutputEntry"));
    asymSignOutputButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignOutputButton"));
    asymSignKeyEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignKeyEntry"));
    asymSignKeyButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignKeyButton"));

    g_signal_connect(asymSignInputChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSignInputEntry);
    g_signal_connect(asymSignInputButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSignInputChooserDialog);
    g_signal_connect(asymSignOutputFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSignOutputEntry);
    g_signal_connect(asymSignOutputButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSignOutputFileChooserDialog);
    g_signal_connect(asymSignKeyChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSignKeyEntry);
    g_signal_connect(asymSignKeyButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSignKeyChooserDialog);

    asymSignDigestDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSignDigestDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymSignDigestDropDown), G_LIST_MODEL(gtk_string_list_new(settings_dialog_get_derive_algorithmValues())));

    asymBase64Check = GTK_WIDGET(gtk_builder_get_object(builder, "asymBase64Check"));

    asymKeyPrivAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivAlgorithmDropDown"));
    asymKeyPrivBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivBlockModeDropDown"));
    asymKeyPrivKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(asymKeyPrivAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    asymmetric_sign_wrapper_provide_widgets(wrapper, asymSignInputEntry, asymSignOutputEntry, asymSignKeyEntry,
                                            asymSignDigestDropDown, asymBase64Check,
                                            asymKeyPrivAlgorithmDropDown, asymKeyPrivBlockModeDropDown, asymKeyPrivKeySizeDropDown);
    asymmetric_sign_data_provide_dialogs(data, progressBox, alertDialog, passwordDialog);

    return wrapper;
}
