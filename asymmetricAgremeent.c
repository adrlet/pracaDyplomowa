#include <pthread.h>

#include "asymmetricAgremeent.h"
#include "asymmetricAgremeent_struct.h"
#include "asymmetricAgremeentPanel_struct.h"
#include "pkey_async_agremeent.h"
#include "pkey_async_agremeent_struct.h"

#include "thread_asynchronichandler.h"
#include "message_digest_map.h"
#include "asymmetric_algorithm_map.h"
#include "symmetric_cipher_map.h"
#include "widgetManagment.h"
#include "widgetAsync.h"
#include "jobsLog.h"
#include "log.h"
#include "passwordFilter.h"

const char * assymetricAgreeOperation = "asymmetric_sign";

gboolean asymmetric_agremeent_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data)
{
    return TRUE;
}

static void asymmetric_agremeent_ok(struct asymmetric_agremeent_data * data, const char * operationString)
{
    printf("asymmetric_agremeent_ok\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Operacja zakończona pomyślnie");
}

static void asymmetric_agremeent_error(struct asymmetric_agremeent_data * data, int errorCode, const char * operationString)
{
    printf("asymmetric_agremeent_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Niepowodzenie, kod błędu: %d", errorCode);
}

static void _asymmetric_agremeent_progress_finish(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data)
{
    const char * operationString = "Uzgadnianie";

    struct asymmetric_agremeent_data * data = (struct asymmetric_agremeent_data*)user_data;

    gtk_widget_set_visible(GTK_WIDGET(spinnerDialog), FALSE);

    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    printf("_asymmetric_agremeent_progress_finish %d\n", iResult);

    if(iResult == 1)
        asymmetric_agremeent_ok(data, operationString);
    else
        asymmetric_agremeent_error(data, -iResult, operationString);

    gtk_window_present(GTK_WINDOW(data->alertDialog));
}

static int _asymmetric_agremeent_progress_func(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct asymmetric_agremeent_data * data = (struct asymmetric_agremeent_data*)user_data;

    printf("_asymmetric_agremeent_progress_func\n");

    int * iResult = NULL;
    int realResult = 0;

    GtkWidget * spinnerDialog2 = GTK_WIDGET(spinnerDialog);
    GtkWidget * spinnerDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(spinnerDialog2));
    GtkWidget * spinnerLabel = NULL;
    GtkWidget * spinner = NULL;

    struct spinner_async_provider * spinner_provider = NULL;

    pthread_t thread;
    struct async_agremeent_data * async_data = NULL;

    spinnerLabel = get_child_of_type(spinnerDialogContent, GTK_TYPE_LABEL, 0);
    if(spinnerLabel == NULL)
        goto error;

    spinner = get_child_of_type(spinnerLabel, GTK_TYPE_SPINNER, 1);
    if(spinner == NULL)
        goto error;

    async_data = async_agremeent_data_new();
    if(async_data == NULL)
        goto error;

    async_agremeent_data_provide(async_data, data->keyPath, data->privKeyPath, data->pubKeyPath, data->outputPath,
                                 data->passphrase,
                                 CIPHER_FULL_ADDRESS(data->privAlgorithm, data->privKeySize, data->privBlockMode), data->flags);

    spinner_async_provider_new(spinner_provider, spinner, TRUE);

    spinner_provider->instances++;
    g_idle_add(spinner_async_update, spinner_provider);

    pthread_create(&thread, NULL, async_agremeent, async_data);
    pthread_join(thread, (void**)&iResult);

    spinner_provider->spin = FALSE;
    spinner_provider->instances++;
    g_idle_add(spinner_async_update, spinner_provider);

error:

//end:
    if(iResult != NULL)
    {
        realResult = iResult[0];
        free(iResult);
    }
    else
        realResult = 0;

    if(realResult == 1)
        jobs_log_async_write(assymetricAgreeOperation, 0);
    else
        jobs_log_async_write(assymetricAgreeOperation, 1);

    if(spinner_provider != NULL)
    {
        if(spinner_provider->instances == 0)
        {
            free(spinner_provider);
            printf("spinner idle freed\n");
        }
        else
            spinner_provider->spinner = NULL;
    }

    if(async_data != NULL)
        async_agremeent_data_delete(async_data);

    return realResult;
}

static void _asymmetric_agremeent_progress_thread(GTask * task, void * spinnerDialog, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_asymmetric_agremeent_progress_thread\n");

    iResult = _asymmetric_agremeent_progress_func(spinnerDialog, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _asymmetric_agremeent_progress_async(GtkWidget * spinnerDialog, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(spinnerDialog, cancellable, arc, user_data);

    printf("_asymmetric_agremeent_progress_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)asymmetric_agremeent_data_clear);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _asymmetric_agremeent_progress_thread);

    g_object_unref(task);
}

static void asymmetric_agremeent_progress_init(gpointer user_data)
{
    const char * operationString = "Uzgadnianie...";

    struct asymmetric_agremeent_data * data = (struct asymmetric_agremeent_data*)user_data;

    printf("asymmetric_agremeent_progress_init\n");

    gtk_window_set_title(GTK_WINDOW(data->spinnerDialog), operationString);
    gtk_window_present(GTK_WINDOW(data->spinnerDialog));

    _asymmetric_agremeent_progress_async(data->spinnerDialog, NULL, _asymmetric_agremeent_progress_finish, user_data);
}

void asymmetric_agremeent_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data)
{
    if(response_id != GTK_RESPONSE_OK)
        return;

    struct asymmetric_agremeent_data * data = (struct asymmetric_agremeent_data*)user_data;

    printf("asymmetric_agremeent_password_cb\n");

    GtkWidget * passwordDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(passwordDialog));
    GtkWidget * passwordFilterLabel = NULL;
    GtkWidget * passwordEntry = NULL;
    GtkWidget * passwordRepeatEntry = NULL;

    const char * password = NULL;
    const char * passwordRepeat = NULL;
    int passwordResult = 0;

    passwordFilterLabel = get_child_of_type(passwordDialogContent, GTK_TYPE_LABEL, 0);
    if(passwordFilterLabel == NULL)
        return;

    passwordEntry = get_child_of_type(passwordFilterLabel, GTK_TYPE_PASSWORD_ENTRY, 1);
    if(passwordEntry == NULL)
        return;

    passwordRepeatEntry = get_child_of_type(passwordEntry, GTK_TYPE_PASSWORD_ENTRY, 1);
    if(passwordRepeatEntry == NULL)
        return;

    password = gtk_editable_get_text(GTK_EDITABLE(passwordEntry));
    passwordRepeat = gtk_editable_get_text(GTK_EDITABLE(passwordRepeatEntry));

    passwordResult = check_password(password, passwordRepeat);
    switch(passwordResult)
    {
    case PASS_TOO_SHORT:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło ma mniej niż 8 znaków");
        break;
    case PASS_TOO_LONG:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło ma więcej niż 256 znaków");
        break;
    case PASS_DOESNT_CONTAIN_SPECIAL_CHAR:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada znaku specjalnego");
        break;
    case PASS_DOESNT_CONTAIN_LOW_LETTER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada małej litery");
        break;
    case PASS_DOESNT_CONTAIN_UP_LETTER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada wielkiej litery");
        break;
     case PASS_DOESNT_CONTAIN_NUMBER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada cyfry");
        break;
    case PASS_IS_EMPTY:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło jest puste");
        break;
    case PASS_RETRY_IS_EMPTY:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Powtórz hasło");
        break;
    case PASS_AND_PASS_RETRY_ARE_DIFFERENT:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie zgadzają się");
        break;
    default:
    }

    if(passwordResult != PASS_IS_OK)
        return;

    asymmetric_agremeent_data_provide_password(data, password);

    gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordEntry), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordRepeatEntry), "");
    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);

    asymmetric_agremeent_progress_init(user_data);
}

gboolean asymmetric_agremeent_password_close_cb(GtkDialog * passwordDialog, gpointer user_data)
{
    struct asymmetric_agremeent_data * data = (struct asymmetric_agremeent_data*)user_data;

    asymmetric_agremeent_data_clear(data);

    printf("asymmetric_agremeent_password_close_cb\n");

    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);
    return TRUE;
}

static void asymmetric_agremeent_proceed(gpointer user_data, unsigned char mode)
{
    struct asymmetric_agremeent_wrapper * wrapper = (struct asymmetric_agremeent_wrapper*)user_data;
    struct asymmetric_agremeent_data * data = wrapper->data;

    int iResult = 0;

    if(data->flags & IN_USE)
        return;

    printf("asymmetric_agremeent_proceed\n");

    asymmetric_agremeent_data_provide_flags(data, mode | IN_USE);
    asymmetric_agremeent_wrapper_provide_data(wrapper, data);
    iResult = asymmetric_agremeent_data_verify(data);
    if(iResult != ASSYMETRIC_AGREMEENT_OK)
    {
        asymmetric_agremeent_data_clear(data);
        return;
    }

    //asymmetric_agremeent_progress_init(data);
    gtk_window_present(GTK_WINDOW(data->passwordDialog));
}

void asymmetric_agremeent_gen(GtkButton * paramGenButton, gpointer user_data)
{
    printf("asymmetric_agremeent_gen\n");
    asymmetric_agremeent_proceed(user_data, 0);
}
