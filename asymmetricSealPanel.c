#include "asymmetricSealPanel.h"
#include "asymmetricSealPanel_struct.h"
#include "asymmetricSeal.h"
#include "asymmetricSeal_struct.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "message_digest_map.h"
#include "asymmetric_algorithm_map.h"
#include "symmetric_cipher_map.h"
#include "progressDialog.h"

/// Wczytanie panelu

struct asymmetric_seal_wrapper * asymmetric_seal_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "asymmetricSeal.xml";

    // Uchwyty
    struct asymmetric_seal_wrapper * wrapper = NULL;
    struct asymmetric_seal_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * asymSealInputEntry = NULL;
    GtkWidget * asymSealInputButton = NULL;
    GtkWidget * asymSealOutputEntry = NULL;
    GtkWidget * asymSealOutputButton = NULL;
    GtkWidget * asymSealPubKeysEntry = NULL;
    GtkWidget * asymSealPubKeysButton = NULL;
    GtkWidget * asymSealPrivKeysEntry = NULL;
    GtkWidget * asymSealPrivKeysButton = NULL;
    GtkWidget * asymSealIvEntry = NULL;
    GtkWidget * asymSealIvButton = NULL;
    GtkWidget * asymSealSessionEntry = NULL;
    GtkWidget * asymSealSessionButton = NULL;

    // Dialogi
    GtkWidget * asymSealInputChooserDialog = NULL;
    GtkWidget * asymSealOutputFileChooserDialog = NULL;
    GtkWidget * asymSealPubKeysChooserDialog = NULL;
    GtkWidget * asymSealPrivKeysChooserDialog = NULL;
    GtkWidget * asymSealIvChooserDialog = NULL;
    GtkWidget * asymSealSessionChooserDialog = NULL;
    GFile * curFolder = NULL;

    GtkWidget * progressDialog = NULL;
    GtkWidget * alertDialog = NULL;
    GtkWidget * passwordDialog = NULL;

    GtkWidget * progressDialogContent = NULL;
    GtkWidget * progressBox = NULL;
    GtkWidget * progressButton = NULL;

    // Inne dane
    GtkWidget * asymKeyAlgorithmDropDown = NULL;
    GtkWidget * asymKeyBlockModeDropDown = NULL;
    GtkWidget * asymKeyKeySizeDropDown = NULL;

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

    wrapper = asymmetric_seal_wrapper_new();
    data = asymmetric_seal_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    asymmetric_seal_wrapper_provide_window(wrapper, mainWindow);
    asymmetric_seal_data_provide_window(data, mainWindow);

    // Dialogi plików

    asymSealInputChooserDialog = gtk_file_chooser_dialog_new("Wejście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealInputChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSealOutputFileChooserDialog = gtk_file_chooser_dialog_new("Wyjście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealOutputFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSealPubKeysChooserDialog = gtk_file_chooser_dialog_new("Klucze publiczne", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealPubKeysChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSealPrivKeysChooserDialog = gtk_file_chooser_dialog_new("Klucz prywatny", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealPrivKeysChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSealIvChooserDialog = gtk_file_chooser_dialog_new("Wektor Inicjujący", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealIvChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymSealSessionChooserDialog = gtk_file_chooser_dialog_new("Klucze sesji", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymSealSessionChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    curFolder = g_file_new_for_path(settings_get_settings()->keysPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymSealPubKeysChooserDialog), curFolder, NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymSealPrivKeysChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);

    // Dialog progress
    progressDialog = progress_dialog_get_dialog();
    progressDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(progressDialog));

    progressBox = progress_dialog_get_template();
    progressButton = get_child_of_type(progressBox, GTK_TYPE_BUTTON, 0);
    g_signal_connect(progressButton, "clicked", G_CALLBACK(_asymmetric_seal_progress_cancel_cb), data);
    gtk_box_append(GTK_BOX(progressDialogContent), progressBox);
    g_object_unref(progressBox);
    gtk_widget_set_visible(progressBox, FALSE);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Dialog Password
    passwordDialog = GTK_WIDGET(gtk_builder_get_object(builder, "passwordDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(passwordDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(passwordDialog, "response", G_CALLBACK(asymmetric_seal_password_cb), data);
    g_signal_connect(passwordDialog, "close-request", G_CALLBACK(asymmetric_seal_password_close_cb), data);

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    asymSealInputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealInputEntry"));
    asymSealInputButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealInputButton"));
    asymSealOutputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealOutputEntry"));
    asymSealOutputButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealOutputButton"));
    asymSealPubKeysEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealPubKeysEntry"));
    asymSealPubKeysButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealPubKeysButton"));
    asymSealPrivKeysEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealPrivKeysEntry"));
    asymSealPrivKeysButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealPrivKeysButton"));
    asymSealIvEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealIvEntry"));
    asymSealIvButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealIvButton"));
    asymSealSessionEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealSessionEntry"));
    asymSealSessionButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealSessionButton"));

    g_signal_connect(asymSealInputChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealInputEntry);
    g_signal_connect(asymSealInputButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealInputChooserDialog);
    g_signal_connect(asymSealOutputFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealOutputEntry);
    g_signal_connect(asymSealOutputButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealOutputFileChooserDialog);
    g_signal_connect(asymSealPubKeysChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealPubKeysEntry);
    g_signal_connect(asymSealPubKeysButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealPubKeysChooserDialog);
    g_signal_connect(asymSealPrivKeysChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealPrivKeysEntry);
    g_signal_connect(asymSealPrivKeysButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealPrivKeysChooserDialog);
    g_signal_connect(asymSealIvChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealIvEntry);
    g_signal_connect(asymSealIvButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealIvChooserDialog);
    g_signal_connect(asymSealSessionChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymSealSessionEntry);
    g_signal_connect(asymSealSessionButton, "clicked", G_CALLBACK(symOutputButton_cb), asymSealSessionChooserDialog);

    asymKeyAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealEncryptAlgorithmDropDown"));
    asymKeyBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealEncryptBlockModeDropDown"));
    asymKeyKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealEncryptKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(asymKeyAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    asymBase64Check = GTK_WIDGET(gtk_builder_get_object(builder, "asymBase64Check"));

    asymKeyPrivAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealAlgorithmDropDown"));
    asymKeyPrivBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealBlockModeDropDown"));
    asymKeyPrivKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymSealKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(asymKeyPrivAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    asymmetric_seal_wrapper_provide_widgets(wrapper, asymSealInputEntry, asymSealOutputEntry, asymSealPubKeysEntry, asymSealPrivKeysEntry,
                                            asymSealIvEntry, asymSealSessionEntry, asymKeyAlgorithmDropDown, asymKeyBlockModeDropDown,
                                            asymKeyKeySizeDropDown, asymBase64Check, asymKeyPrivAlgorithmDropDown, asymKeyPrivBlockModeDropDown,
                                            asymKeyPrivKeySizeDropDown);
    asymmetric_seal_data_provide_dialogs(data, progressBox, alertDialog, passwordDialog);

    return wrapper;
}
