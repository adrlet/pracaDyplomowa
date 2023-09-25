#include <pthread.h>

#include "symmetricDigest.h"
#include "symmetricDigest_struct.h"
#include "symmetricDigestPanel_struct.h"

#include "evp_async_digest.h"
#include "evp_async_digest_struct.h"

#include "message_digest_map.h"
#include "widgetManagment.h"

#include "thread_asynchronichandler.h"
#include "widgetAsync.h"
#include "jobsLog.h"
#include "log.h"

const char * digestOperation = "Digest";

void _symmetric_digest_progress_cancel_cb(GtkButton * progressButton, gpointer user_data)
{
    struct symmetric_digest_data * data = (struct symmetric_digest_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(data->progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    printf("_symmetric_digest_progress_cancel_cb\n");

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    g_cancellable_cancel(data->cancel);
    gtk_widget_set_visible(GTK_WIDGET(data->progressBox), FALSE);
}

static void symmetric_digest_ok(struct symmetric_digest_data * data, const char * filename, const char * operationString)
{
    printf("symmetric_digest_ok\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Operacja zakończona pomyślnie %s", filename);
}

static void symmetric_digest_error(struct symmetric_digest_data * data, const char * filename, int errorCode, const char * operationString)
{
    printf("symmetric_digest_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    if(errorCode == 1)
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Przerwano");
    else
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Niepowodzenie, kod błędu: %d", errorCode);
}

static void _symmetric_digest_progress_finish(GObject * progressBox, GAsyncResult * res, gpointer user_data)
{
    struct symmetric_digest_data * data = (struct symmetric_digest_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    char * inputFileName = NULL;
    const char * operationString = "Generowanie skrótu";

    printf("_symmetric_digest_progress_finish %d\n", iResult);

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    gtk_widget_set_visible(GTK_WIDGET(progressBox), FALSE);

    inputFileName = getFileName(data->inputPath);

    if(iResult == 1)
        symmetric_digest_ok(data, inputFileName, operationString);
    else
        symmetric_digest_error(data, inputFileName, -iResult, operationString);

    if(inputFileName != NULL)
        free(inputFileName);

    gtk_window_present(GTK_WINDOW(data->alertDialog));
}

static int _symmetric_digest_progress_func(GObject * progressBox, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct symmetric_digest_data * data = (struct symmetric_digest_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    int * iResult = NULL;
    int realResult = 0;
    pthread_t thread;

    struct progress_bar_async_provider * progress_bar_provider = NULL;
    struct label_async_provider * label_provider = NULL;

    clock_t interval;
    double progress = 0.0;
    long double fileSize = 0.0;
    char * progressPercent = NULL;
    const int progressPercentLen = 15;

    struct asynchronicHandler * handler = NULL;
    struct async_digest_data * digest_data = NULL;

    printf("_symmetric_encode_progress_func\n");

    fileSize = (long double)getFileSize(data->inputPath);
    progressPercent = (char*)malloc(sizeof(char)*(progressPercentLen+1));
    strcpy(progressPercent, logs_get_progressText());

    progress_bar_async_provider_new(progress_bar_provider, progressBar, &progress);
    label_async_provider_new(label_provider, progressLabel, progressPercent);

    handler = setupAsynchronicHandler();
    if(handler == NULL)
        goto end;

    digest_data = async_digest_data_new();
    if(digest_data == NULL)
        goto end;

    async_digest_data_provide(digest_data, data->inputPath, data->outputPath, NULL, handler, data->algorithm, data->flags);
    pthread_create(&thread, NULL, async_digest, digest_data);

    interval = clock();
    while(progress < 1.0)
    {
        while(clock()-interval < CLOCKS_PER_SEC)
            continue;
        interval = clock();

        progress = (double)(((long double)handler->processed)/fileSize);
        sprintf(progressPercent+logs_get_progressTextLen(), "%0.f%%", round(progress*100.f));

        if(g_cancellable_is_cancelled(cancellable))
            handler->interrupt = 1;
        if(handler->interrupt == 1)
            goto interrupted;

        progress_bar_provider->instances++;
        g_idle_add(progress_bar_async_update, progress_bar_provider);

        label_provider->instances++;
        g_idle_add(label_async_update, label_provider);

        printf("Processed: %llu\n", handler->processed);
    }

interrupted:
    pthread_join(thread, (void**)&iResult);

error:

end:
    if(iResult != NULL)
    {
        realResult = iResult[0];
        free(iResult);
    }
    else
        realResult = 0;

    switch(realResult)
    {
    case 1:
        jobs_log_async_write(digestOperation, 0);
        break;
    case -1:
        jobs_log_async_write(digestOperation, 2);
        break;
    default:
        jobs_log_async_write(digestOperation, 1);
    }

    if(progress_bar_provider != NULL)
    {
        if(progress_bar_provider->instances == 0)
        {
            free(progress_bar_provider);
            printf("progress bar idle freed\n");
        }
        else
            progress_bar_provider->progressBar = NULL;
    }
    if(label_provider != NULL)
    {
        if(label_provider->instances == 0)
        {
            free(label_provider);
            printf("label idle freed\n");
        }
        else
            label_provider->label = NULL;
    }

    if(progressPercent != NULL)
        free(progressPercent);
    if(handler != NULL)
        freeAsynchronicHandler(handler);
    if(digest_data != NULL)
        async_digest_data_delete(digest_data);

    return realResult;
}

static void _symmetric_digest_progress_thread(GTask * task, void * progressBox, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_symmetric_digest_progress_thread\n");

    iResult = _symmetric_digest_progress_func(progressBox, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _symmetric_digest_progress_async(GtkWidget * progressBox, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(progressBox, cancellable, arc, user_data);

    printf("_symmetric_encode_progress_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)symmetric_digest_data_clear);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _symmetric_digest_progress_thread);

    g_object_unref(task);
}

static void symmetric_digest_progress_init(gpointer user_data)
{
    static const char * operationString = "Tworzenie skrótu trwa...";

    struct symmetric_digest_data * data = (struct symmetric_digest_data*)user_data;
    GtkWidget * progressBox = data->progressBox;
    GtkWidget * progressName = get_child_of_type(data->progressBox, GTK_TYPE_LABEL, 0);

    printf("symmetric_digest_progress_init\n");

    gtk_label_set_text(GTK_LABEL(progressName), operationString);

    gtk_widget_set_visible(progressBox, TRUE);

    _symmetric_digest_progress_async(progressBox, data->cancel, _symmetric_digest_progress_finish, user_data);
}

void symmetric_digest_cb(GtkButton * digestButton, gpointer user_data)
{
    printf("symmetric_digest_cb\n");

    struct symmetric_digest_wrapper * wrapper = (struct symmetric_digest_wrapper*)user_data;
    struct symmetric_digest_data * data = wrapper->data;

    int iResult = 0;

    if(data->flags & IN_USE)
        return;

    symmetric_digest_data_provide_flags(data, IN_USE);
    symmetric_digest_wrapper_provide_data(wrapper, data);
    iResult = symmetric_digest_data_verify(data);
    if(iResult != SYMMETRIC_DIGEST_OK)
    {
        symmetric_digest_data_clear(data);
        return;
    }

    symmetric_digest_progress_init(data);
}
