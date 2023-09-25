#include <pthread.h>

#include "asymmetricParamPanel.h"
#include "asymmetricParam_struct.h"
#include "asymmetricParamPanel_struct.h"
#include "pkey_async_param.h"
#include "pkey_async_param_struct.h"

#include "thread_asynchronichandler.h"
#include "symmetric_cipher_map.h"
#include "widgetManagment.h"
#include "widgetAsync.h"
#include "jobsLog.h"

const char * assymetricParamOperation = "asymmetric_param";

gboolean asymmetric_param_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data)
{
    return TRUE;
}

static void asymmetric_param_ok(struct asymmetric_param_data * data, const char * operationString)
{
    printf("asymmetric_param_ok\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Operacja zakończona pomyślnie");
}

static void asymmetric_param_error(struct asymmetric_param_data * data, int errorCode, const char * operationString)
{
    printf("asymmetric_param_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Niepowodzenie, kod błędu: %d", errorCode);
}

static void _asymmetric_param_progress_finish(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data)
{
    const char * paramCheck = "Sprawdzanie parametrów";
    const char * paramGen = "Generowanie parametrów";
    const char * operationString = NULL;
    int mode = 0;

    struct asymmetric_param_data * data = (struct asymmetric_param_data*)user_data;

    gtk_widget_set_visible(GTK_WIDGET(spinnerDialog), FALSE);

    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    printf("_asymmetric_param_progress_finish %d\n", iResult);

    mode = (data->flags & ASSYMETRIC_PARAM_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_PARAM_GEN:
        operationString = paramGen;
        break;
    case ASSYMETRIC_PARAM_CHECK:
        operationString = paramCheck;
        break;
    }

    if(iResult == 1)
        asymmetric_param_ok(data, operationString);
    else
        asymmetric_param_error(data, -iResult, operationString);

    gtk_window_present(GTK_WINDOW(data->alertDialog));
}

static int _asymmetric_param_progress_func(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct asymmetric_param_data * data = (struct asymmetric_param_data*)user_data;

    printf("_asymmetric_param_progress_func\n");

    int * iResult = NULL;
    int realResult = 0;
    int mode = 0;

    GtkWidget * spinnerDialog2 = GTK_WIDGET(spinnerDialog);
    GtkWidget * spinnerDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(spinnerDialog2));
    GtkWidget * spinnerLabel = NULL;
    GtkWidget * spinner = NULL;

    struct spinner_async_provider * spinner_provider = NULL;

    pthread_t thread;
    struct async_param_data * async_data = NULL;

    spinnerLabel = get_child_of_type(spinnerDialogContent, GTK_TYPE_LABEL, 0);
    if(spinnerLabel == NULL)
        goto error;

    spinner = get_child_of_type(spinnerLabel, GTK_TYPE_SPINNER, 1);
    if(spinner == NULL)
        goto error;

    async_data = async_param_data_new();
    if(async_data == NULL)
        goto error;

    async_param_data_provide(async_data, data->paramPath, data->keySize, data->algorithm, data->flags);

    spinner_async_provider_new(spinner_provider, spinner, TRUE);

    spinner_provider->instances++;
    g_idle_add(spinner_async_update, spinner_provider);

    mode = (data->flags & ASSYMETRIC_PARAM_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_PARAM_GEN:
        pthread_create(&thread, NULL, async_param_gen, async_data);
        break;
    case ASSYMETRIC_PARAM_CHECK:
        pthread_create(&thread, NULL, async_param_check, async_data);
        break;
    }

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
        jobs_log_async_write(assymetricParamOperation, 0);
    else
        jobs_log_async_write(assymetricParamOperation, 1);

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
        async_param_data_delete(async_data);

    return realResult;
}

static void _asymmetric_param_progress_thread(GTask * task, void * spinnerDialog, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_asymmetric_param_progress_thread\n");

    iResult = _asymmetric_param_progress_func(spinnerDialog, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _asymmetric_param_progress_async(GtkWidget * spinnerDialog, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(spinnerDialog, cancellable, arc, user_data);

    printf("_asymmetric_param_progress_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)asymmetric_param_data_clear);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _asymmetric_param_progress_thread);

    g_object_unref(task);
}

static void asymmetric_param_progress_init(gpointer user_data)
{
    const char * operationString = NULL;
    static const char * paramCheck = "Sprawdzanie parametrów...";
    static const char * paramGen = "Generowanie parametrów...";
    int mode = 0;

    struct asymmetric_param_data * data = (struct asymmetric_param_data*)user_data;

    printf("asymmetric_param_progress_init\n");

    mode = (data->flags & ASSYMETRIC_PARAM_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_PARAM_GEN:
        operationString = paramGen;
        break;
    case ASSYMETRIC_PARAM_CHECK:
        operationString = paramCheck;
        break;
    }

    gtk_window_set_title(GTK_WINDOW(data->spinnerDialog), operationString);
    gtk_window_present(GTK_WINDOW(data->spinnerDialog));

    _asymmetric_param_progress_async(data->spinnerDialog, NULL, _asymmetric_param_progress_finish, user_data);
}

static void asymmetric_param_proceed(gpointer user_data, unsigned char mode)
{
    struct asymmetric_param_wrapper * wrapper = (struct asymmetric_param_wrapper*)user_data;
    struct asymmetric_param_data * data = wrapper->data;

    int iResult = 0;

    if(data->flags & IN_USE)
        return;

    printf("asymmetric_param_proceed\n");

    asymmetric_param_data_provide_flags(data, mode | IN_USE);
    asymmetric_param_wrapper_provide_data(wrapper, data);
    iResult = asymmetric_param_data_verify(data);
    if(iResult != ASSYMETRIC_PARAM_OK)
    {
        asymmetric_param_data_clear(data);
        return;
    }

    asymmetric_param_progress_init(data);
}

void asymmetric_param_gen(GtkButton * paramGenButton, gpointer user_data)
{
    printf("asymmetric_param_gen\n");
    asymmetric_param_proceed(user_data, ASYMMETRIC_PARAM_GEN);
}

void asymmetric_param_check(GtkButton * paramCheckButton, gpointer user_data)
{
    printf("asymmetric_param_check\n");
    asymmetric_param_proceed(user_data, ASSYMETRIC_PARAM_CHECK);
}
