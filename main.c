#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "bio_init.h"

#include "symmetricPanel.h"
#include "symmetricPanel_struct.h"
#include "symmetricEncrypt.h"
#include "symmetricDigestPanel.h"
#include "symmetricDigestPanel_struct.h"
#include "symmetricDigest.h"
#include "asymmetricKeyPanel.h"
#include "asymmetricKey.h"
#include "asymmetricParamPanel.h"
#include "asymmetricParam.h"
#include "asymmetricSignPanel.h"
#include "asymmetricSign.h"
#include "asymmetricAgremeentPanel.h"
#include "asymmetricAgremeent.h"
#include "asymmetricSealPanel.h"
#include "asymmetricSeal.h"

#include "widgetManagment.h"
#include "progressDialog.h"
#include "errorDialog.h"
#include "jobsDialog.h"
#include "settings.h"
#include "settingsDialog.h"

const char APPLICATION_ID[] = "gtk4.openssl.interface";

const char * const functionsGroupString[] =
{
    "Szyfrowanie symetryczne", "Skrót wiadomości",
    "Zarządzanie kluczami", "Zarządzanie parametrami",
    "Podpis cyfrowy",
    "Uzgadnianie kluczy", "Współdzielenie kluczy",
    NULL
};

const char * const functionalitiesSymetric[] = { "Zaszyfruj", "Odszyfruj", NULL };
const char * const functionalitiesSymetricDigest[] = { "Wygeneruj skrót", NULL };
const char * const functionalitiesAsymetricKey[] = { "Generowanie kluczy", "Generowanie kluczy z parametru", "Sprawdzenie pary", NULL };
const char * const functionalitiesAsymetricParam[] = { "Generowanie parametrów", "Sprawdzenie parametrów", NULL };
const char * const functionalitiesAsymetricSign[] = { "Podpisz plik", "Sprawdź podpis", NULL };
const char * const functionalitiesAsymetricAgree[] = { "Uzgodnij klucz", NULL };
const char * const functionalitiesAsymetricSeal[] = { "Utwórz kopertę", "Otwórz kopertę", NULL };

const char * const * const functionalitiesString[] =
{
    functionalitiesSymetric,
    functionalitiesSymetricDigest,
    functionalitiesAsymetricKey,
    functionalitiesAsymetricParam,
    functionalitiesAsymetricSign,
    functionalitiesAsymetricAgree,
    functionalitiesAsymetricSeal,
    NULL
};

// Handlery fabryki listy
void setup_cb(GtkListItemFactory * factory, GtkListItem * listitem, gpointer user_data)
{
    unsigned char vertical = *(unsigned char*)user_data;

    GtkWidget * label = gtk_label_new("");
    GtkWidget * button = gtk_button_new();

    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(label), GTK_WRAP_WORD_CHAR);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_button_set_child(GTK_BUTTON(button), label);

    if(vertical)
    {
        gtk_label_set_max_width_chars(GTK_LABEL(label), 12);
        gtk_widget_set_size_request(button, 108, 48);
    }
    else
    {
        gtk_label_set_max_width_chars(GTK_LABEL(label), 18);
        gtk_widget_set_size_request(button, 96, 44);
    }

    gtk_widget_add_css_class(button, "functionsButton");
    gtk_list_item_set_child(listitem, button);
}

void bind_cb(GtkSignalListItemFactory * self, GtkListItem * listitem, gpointer user_data)
{
    GtkWidget * button = gtk_list_item_get_child (listitem);
    GtkWidget * label = gtk_button_get_child(GTK_BUTTON(button));
    GtkStringObject * strobj = gtk_list_item_get_item (listitem);
    const char * text = gtk_string_object_get_string (strobj);

    int position = 0;
    struct functionalities_pointer * functions = NULL;
    void (*functionPointer)(GtkButton*, gpointer) = NULL;

    gtk_label_set_text(GTK_LABEL(label), text);

    // Podpiecie handlera do sygnalu guzika
    position = gtk_list_item_get_position(listitem);
    functions = (struct functionalities_pointer*)user_data;
    functionPointer = functions->functions[position];

    g_signal_connect(button, "clicked", G_CALLBACK(functionPointer), functions->data);
}

void unbind_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data)
{
    struct functionalities_pointer * functions = (struct functionalities_pointer*)user_data;
    GtkWidget * button = gtk_list_item_get_child (listitem);

    g_signal_handlers_disconnect_by_data(button, functions->data);
}

void teardown_cb(GtkListItemFactory *factory, GtkListItem *listitem, gpointer user_data)
{
    gtk_list_item_set_child (listitem, NULL);
}

void app_activate(GApplication * application, gpointer * userData)
{
    // iteratory
    int i = 0;

    // zmienne pomocnicze
    unsigned char vertical = 0;

    /// okno
    GtkBuilder * builder = NULL;

    GtkWidget * mainWindow = NULL;
    GMenuModel * menuBar = NULL;
    GFile * cssFile = NULL;
    GdkDisplay * mainWindowDisplay = NULL;
    GtkCssProvider * cssProvider = NULL;

    /// główna treść
    // przypadki użycia
    GtkWidget * functionsGroupNotepad = NULL;

    // funkcje
    struct functionalities_pointer ** functionalitiesPointers = NULL;
    struct functionalities_pointer * functionalityPointer = NULL;
    void (**functions)(GtkButton*, gpointer) = NULL;

    //struct symmetric_wrapper * symWrap = NULL;

    GtkWidget * functionSectionBox = NULL;
    GtkWidget * functionsGroupNotepadLabel = NULL;
    GtkStringList * functionalitiesStringList = NULL;
    GtkNoSelection * functionalitiesNoSelection = NULL;
    GtkListItemFactory * functionalitiesFactory = NULL;
    GtkWidget * functionalitiesListView = NULL;

    // Inicjalizacja
    functionalitiesPointers = (struct functionalities_pointer**)malloc(sizeof(struct functionalities_pointer*)*8);
    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*2);

    functions[0] = symmetric_encode_cb;
    functions[1] = symmetric_decode_cb;

    functionalityPointer->functions = functions;

    functionalitiesPointers[0] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*1);

    functions[0] = symmetric_digest_cb;

    functionalityPointer->functions = functions;

    functionalitiesPointers[1] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*3);

    functions[0] = asymmetric_key_rsa_gen;
    functions[1] = asymmetric_key_on_param_gen;
    functions[2] = asymmetric_key_check;

    functionalityPointer->functions = functions;

    functionalitiesPointers[2] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*2);

    functions[0] = asymmetric_param_gen;
    functions[1] = asymmetric_param_check;

    functionalityPointer->functions = functions;

    functionalitiesPointers[3] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*2);

    functions[0] = asymmetric_sign_sign;
    functions[1] = asymmetric_sign_verify;

    functionalityPointer->functions = functions;

    functionalitiesPointers[4] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*1);

    functions[0] = asymmetric_agremeent_gen;

    functionalityPointer->functions = functions;

    functionalitiesPointers[5] = functionalityPointer;

    functionalities_pointer_new(functionalityPointer);
    functions = (void (**)(GtkButton*, gpointer))malloc(sizeof(void (*)(GtkButton*, gpointer))*2);

    functions[0] = asymmetric_seal_seal;
    functions[1] = asymmetric_seal_open;

    functionalityPointer->functions = functions;

    functionalitiesPointers[6] = functionalityPointer;

    functionalitiesPointers[7] = NULL;

    builder = gtk_builder_new_from_file("mainWindow.xml");
    gtk_builder_add_from_file(builder, "menu.xml", NULL);

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    functionsGroupNotepad = GTK_WIDGET(gtk_builder_get_object(builder, "functionsGroupNotepad"));
    functionSectionBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionSectionBox"));
    functionsGroupNotepadLabel = GTK_WIDGET(gtk_builder_get_object(builder, "functionsGroupNotepadLabel"));
    progress_dialog_init(mainWindow);
    error_dialog_init(mainWindow);
    jobs_dialog_init(mainWindow);
    settings_init(mainWindow);

    GSimpleAction * act_progress = g_simple_action_new("progress", NULL);
    g_action_map_add_action (G_ACTION_MAP(mainWindow), G_ACTION(act_progress));
    g_signal_connect_swapped(act_progress, "activate", G_CALLBACK(gtk_widget_show), progress_dialog_get_dialog());

    GSimpleAction * act_errors = g_simple_action_new("errors", NULL);
    g_action_map_add_action (G_ACTION_MAP(mainWindow), G_ACTION(act_errors));
    g_signal_connect_swapped(act_errors, "activate", G_CALLBACK(gtk_widget_show), error_dialog_get_dialog());

    GSimpleAction * act_jobs = g_simple_action_new("jobs", NULL);
    g_action_map_add_action (G_ACTION_MAP(mainWindow), G_ACTION(act_jobs));
    g_signal_connect_swapped(act_jobs, "activate", G_CALLBACK(gtk_widget_show), jobs_dialog_get_dialog());

    GSimpleAction * act_settings = g_simple_action_new("settings", NULL);
    g_action_map_add_action (G_ACTION_MAP(mainWindow), G_ACTION(act_settings));
    g_signal_connect_swapped(act_settings, "activate", G_CALLBACK(settingsDialog_show_cb), settings_dialog_get_dialog());

    menuBar = G_MENU_MODEL(gtk_builder_get_object(builder, "menubar"));
    gtk_application_set_menubar(GTK_APPLICATION(application), menuBar);
    gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(mainWindow), TRUE);

    printf("good\n");

    g_object_unref(builder);

    //  Generowanie panelów dla przypadków użycia
    //for(i = 0; functionsGroupString[i] != NULL; i++)
    for(i = 0; functionalitiesPointers[i] != NULL && functionalitiesString[i] != NULL; i++)
    {
        builder = gtk_builder_new_from_file("notebookPage.xml");

        // Zakładka przypadku użycia
        functionsGroupNotepadLabel = GTK_WIDGET(gtk_builder_get_object(builder, "functionsGroupNotepadLabel"));
        gtk_label_set_text(GTK_LABEL(functionsGroupNotepadLabel), functionsGroupString[i]);

        // Pole przypadku użycia
        functionSectionBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionSectionBox"));

        gtk_notebook_append_page(GTK_NOTEBOOK(functionsGroupNotepad), functionSectionBox, functionsGroupNotepadLabel);

        // Pole ustawień


        switch(i)
        {
        case 0:
            functionalitiesPointers[i]->data = symmetric_create_panel(functionSectionBox);
            break;
        case 1:
            functionalitiesPointers[i]->data = symmetric_digest_create_panel(functionSectionBox);
            break;
        case 2:
            functionalitiesPointers[i]->data = asymmetric_key_create_panel(functionSectionBox);
            break;
        case 3:
            functionalitiesPointers[i]->data = asymmetric_param_create_panel(functionSectionBox);
            break;
        case 4:
            functionalitiesPointers[i]->data = asymmetric_sign_create_panel(functionSectionBox);
            break;
        case 5:
            functionalitiesPointers[i]->data = asymmetric_agremeent_create_panel(functionSectionBox);
            break;
        case 6:
            functionalitiesPointers[i]->data = asymmetric_seal_create_panel(functionSectionBox);
            break;
        }

        // Utworzenie listy funkcjonalnosci
        functionalitiesStringList = gtk_string_list_new((const char * const *)functionalitiesString[i]);
        functionalitiesNoSelection = gtk_no_selection_new(G_LIST_MODEL(functionalitiesStringList));

        functionalitiesFactory = gtk_signal_list_item_factory_new();
        g_signal_connect(functionalitiesFactory, "setup", G_CALLBACK(setup_cb), &vertical);
        g_signal_connect(functionalitiesFactory, "bind", G_CALLBACK(bind_cb), functionalitiesPointers[i]);
        g_signal_connect(functionalitiesFactory, "unbind", G_CALLBACK(unbind_cb), functionalitiesPointers[i]);
        g_signal_connect(functionalitiesFactory, "teardown", G_CALLBACK(teardown_cb), NULL);

        functionalitiesListView = GTK_WIDGET(gtk_builder_get_object(builder, "functionalitiesListView"));
        gtk_list_view_set_factory(GTK_LIST_VIEW(functionalitiesListView), functionalitiesFactory);
        gtk_list_view_set_model(GTK_LIST_VIEW(functionalitiesListView), GTK_SELECTION_MODEL(functionalitiesNoSelection));

        g_object_unref(builder);
    }

    mainWindowDisplay = gtk_widget_get_display(GTK_WIDGET(mainWindow));
    cssProvider = gtk_css_provider_new();
    cssFile = g_file_new_for_path("css.css");

    gtk_css_provider_load_from_file(cssProvider, cssFile);
    gtk_style_context_add_provider_for_display(mainWindowDisplay, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(cssFile);

    gtk_window_set_application (GTK_WINDOW (mainWindow), GTK_APPLICATION (application));

    gtk_window_present(GTK_WINDOW(mainWindow));
}

void app_open(GApplication * application, GFile ** files, gint nFiles, gchar *hint, gpointer userData)
{
    GtkWidget * mainWindow = NULL;

    mainWindow = gtk_application_window_new(GTK_APPLICATION(application));
    gtk_window_set_title(GTK_WINDOW(mainWindow) , APPLICATION_ID);
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 640, 480);

    gtk_window_present(GTK_WINDOW(mainWindow));
}

int main(int argc, char * argv[])
{
    GtkApplication * gtkInterface = NULL;
    int iResult = 0;

    initProcess();

    gtkInterface = gtk_application_new(APPLICATION_ID, G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(gtkInterface, "activate", G_CALLBACK(app_activate), NULL);
    g_signal_connect(gtkInterface, "open", G_CALLBACK(app_open), NULL);
    iResult = g_application_run((GApplication*)gtkInterface, argc, argv);
    g_object_unref(gtkInterface);

    freeProcess();

    return iResult;
}
