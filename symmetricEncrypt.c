#include <pthread.h>

#include "symmetricEncrypt.h"
#include "symmetricEncrypt_struct.h"
#include "evp_async_encryptdecrypt.h"
#include "evp_async_encryptdecrypt_struct.h"
#include "symmetricPanel_struct.h"
#include "symmetricDeriveKey_struct.h"
#include "symmetricDeriveKey.h"

#include "thread_asynchronichandler.h"
#include "symmetric_cipher_map.h"
#include "widgetManagment.h"
#include "widgetAsync.h"
#include "jobsLog.h"
#include "log.h"

const char * encryptOperation = "Encrypt";
const char * decryptOperation = "Decrypt";

void _symmetric_encode_progress_cancel_cb(GtkButton * progressButton, gpointer user_data)
{
    struct symmetric_data * data = (struct symmetric_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(data->progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    printf("_symmetric_encode_progress_cancel_cb\n");

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    g_cancellable_cancel(data->cancel);
    gtk_widget_set_visible(GTK_WIDGET(data->progressBox), FALSE);
}

static void symmetric_encode_ok(struct symmetric_data * data, const char * filename, const char * operationString)
{
    printf("symmetric_encode_ok\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Operacja zakończona pomyślnie %s", filename);
}

static void symmetric_encode_error(struct symmetric_data * data, const char * filename, int errorCode, const char * operationString)
{
    printf("symmetric_encode_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    if(errorCode == 1)
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Przerwano");
    else
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Niepowodzenie, kod błędu: %d", errorCode);
}

static void _symmetric_encode_progress_finish(GObject * progressBox, GAsyncResult * res, gpointer user_data)
{
    const char * encodeString = "Szyfrowanie";
    const char * decodeString = "Rozszyfrowanie";

    struct symmetric_data * data = (struct symmetric_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    char * inputFileName = NULL;
    const char * operationString = NULL;

    printf("_symmetric_encode_progress_finish %d\n", iResult);

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    gtk_widget_set_visible(GTK_WIDGET(progressBox), FALSE);

    inputFileName = getFileName(data->inputPath);

    operationString = (data->flags & SYMMETRIC_MODE_DECRYPT ? decodeString : encodeString);

    if(iResult == 1)
        symmetric_encode_ok(data, inputFileName, operationString);
    else
        symmetric_encode_error(data, inputFileName, -iResult, operationString);

    if(inputFileName != NULL)
        free(inputFileName);

    gtk_window_present(GTK_WINDOW(data->alertDialog));
}

static int _symmetric_encode_progress_func(GObject * progressBox, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct symmetric_data * data = (struct symmetric_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    int * iResult = NULL;
    int realResult = 0;
    pthread_t thread;
    const char * operationString = NULL;

    struct progress_bar_async_provider * progress_bar_provider = NULL;
    struct label_async_provider * label_provider = NULL;

    clock_t interval;
    double progress = 0.0;
    long double fileSize = 0.0;
    char * progressPercent = NULL;
    const int progressPercentLen = 15;

    struct asynchronicHandler * handler = NULL;
    struct async_encrypt_data * encrypt_data = NULL;

    printf("_symmetric_encode_progress_func\n");

    fileSize = (long double)getFileSize(data->inputPath);
    progressPercent = (char*)malloc(sizeof(char)*(progressPercentLen+1));
    strcpy(progressPercent, logs_get_progressText());

    progress_bar_async_provider_new(progress_bar_provider, progressBar, &progress);
    label_async_provider_new(label_provider, progressLabel, progressPercent);

    handler = setupAsynchronicHandler();
    if(handler == NULL)
        goto end;

    encrypt_data = async_encrypt_data_new();
    if(encrypt_data == NULL)
        goto end;

    async_encrypt_data_provide(encrypt_data, data->inputPath, data->outputPath, data->key, data->iv, NULL, handler,
                               CIPHER_FULL_ADDRESS(data->algorithm, data->keysize, data->blockmode), 8, data->flags);
    if(data->flags & SYMMETRIC_MODE_DECRYPT)
    {
        pthread_create(&thread, NULL, async_decrypt, encrypt_data);
        operationString = decryptOperation;
    }
    else
    {
        pthread_create(&thread, NULL, async_encrypt, encrypt_data);
        operationString = encryptOperation;
    }

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
        jobs_log_async_write(operationString, 0);
        break;
    case -1:
        jobs_log_async_write(operationString, 2);
        break;
    default:
        jobs_log_async_write(operationString, 1);
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
    if(encrypt_data != NULL)
        async_encrypt_data_delete(encrypt_data);

    return realResult;
}

static void _symmetric_encode_progress_thread(GTask * task, void * progressBox, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_symmetric_encode_progress_thread\n");

    iResult = _symmetric_encode_progress_func(progressBox, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _symmetric_encode_progress_async(GtkWidget * progressBox, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(progressBox, cancellable, arc, user_data);

    printf("_symmetric_encode_progress_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)symmetric_data_clear);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _symmetric_encode_progress_thread);

    g_object_unref(task);
}

void symmetric_encode_progress_init(gpointer user_data)
{
    static const char * encryptString = "Szyfrowanie trwa...";
    static const char * decryptString = "Rozszyfrowanie trwa...";

    const char * operationString = NULL;

    struct symmetric_data * data = (struct symmetric_data*)user_data;
    GtkWidget * progressBox = data->progressBox;
    GtkWidget * progressName = get_child_of_type(data->progressBox, GTK_TYPE_LABEL, 0);

    printf("symmetric_encode_progress_init\n");

    operationString = (data->flags & SYMMETRIC_MODE_DECRYPT ? decryptString : encryptString);
    gtk_label_set_text(GTK_LABEL(progressName), operationString);

    gtk_widget_set_visible(progressBox, TRUE);

    _symmetric_encode_progress_async(progressBox, data->cancel, _symmetric_encode_progress_finish, user_data);
}

static void symmetric_proceed(gpointer user_data, unsigned char mode)
{
    struct symmetric_wrapper * wrapper = (struct symmetric_wrapper*)user_data;
    struct symmetric_data * data = wrapper->data;
    struct symmetric_derive_key * derive_key_data = wrapper->derive_key_data;

    const char * saltpath = NULL;
    int iResult = 0;
    const EVP_CIPHER * cipher = NULL;

    if((data->flags & IN_USE) || (derive_key_data->flags & IN_USE))
        return;

    printf("symmetric_proceed\n");

    symmetric_data_provide_flags(data, mode | IN_USE);
    symmetric_wrapper_provide_data(wrapper, data);
    iResult = symmetric_data_verify(data);
    cipher = get_symmetric_algorithm(CIPHER_FULL_ADDRESS(data->algorithm, data->keysize, data->blockmode));
    if(cipher == NULL || iResult != SYMMETRIC_PANEL_OK)
    {
        symmetric_data_clear(data);
        return;
    }

    symmetric_derive_key_provide_lengths(derive_key_data, getSymmetricAlgorithmKeyLength(cipher), getSymmetricAlgorithmIvLength(cipher));
    saltpath = (mode == SYMMETRIC_MODE_ENCRYPT ? data->outputPath : data->inputPath);
    symmetric_derive_key_provide_proceed_data(derive_key_data, data->flags, saltpath, 8);

    gtk_window_present(GTK_WINDOW(derive_key_data->passwordDialog));
}

void symmetric_encode_cb(GtkButton * encodeButton, gpointer user_data)
{
    printf("symmetric_encode_cb\n");
    symmetric_proceed(user_data, SYMMETRIC_MODE_ENCRYPT);
}

void symmetric_decode_cb(GtkButton * decodeButton, gpointer user_data)
{
    printf("symmetric_decode_cb\n");
    symmetric_proceed(user_data, SYMMETRIC_MODE_DECRYPT);
}
