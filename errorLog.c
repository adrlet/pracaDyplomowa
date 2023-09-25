#include <string.h>
#include <gtk/gtk.h>
#include <openssl/err.h>
#include <pthread.h>

#include "errorLog.h"
#include "bio_errorhandle.h"
#include "errorDialog.h"

char * errorLogPath = NULL;
GFile * errorLogFile = NULL;
GFileOutputStream * errorLogStream = NULL;

pthread_mutex_t	errorCodeWrite_mutex = PTHREAD_MUTEX_INITIALIZER;

static int error_log_write(int code, const char * openssl_text)
{
    int iResult = 0;
    char * buffer = NULL;
    static const char * newline = "\r\n";
    GDateTime * errorDateTime = NULL;
    char * errorDateTimeString = NULL;

    char * totalBuffer = NULL;
    int totalBufferLength = 0;

    buffer = getErrorString(code);
    if(buffer == NULL)
        goto error;

    errorDateTime = g_date_time_new_now_local();
    errorDateTimeString = g_date_time_format(errorDateTime, "%d/%m/%Y %H:%M:%S");

    totalBufferLength = strlen(errorDateTimeString)+strlen(buffer)+strlen(newline);
    if(openssl_text != NULL)
        totalBufferLength += strlen(openssl_text);

    totalBuffer = (char*)malloc(sizeof(char)*(totalBufferLength+1));
    totalBuffer[0] = 0;
    strcat(totalBuffer, errorDateTimeString);
    strcat(totalBuffer, buffer);
    if(openssl_text != NULL)
        strcat(totalBuffer, openssl_text);
    strcat(totalBuffer, newline);

    pthread_mutex_lock(&errorCodeWrite_mutex);
    iResult = g_output_stream_write(G_OUTPUT_STREAM(errorLogStream), totalBuffer, totalBufferLength, NULL, NULL);
    if(iResult == -1)
        goto error;

    error_dialog_append_string(totalBuffer);

    iResult = 0;
    goto end;

error:
    iResult = -1;

end:
    pthread_mutex_unlock(&errorCodeWrite_mutex);
    if(buffer != NULL)
        free(buffer);
    if(errorDateTimeString != NULL)
        g_free(errorDateTimeString);
    if(totalBuffer != NULL)
        free(totalBuffer);

    return iResult;
}

static int error_log_set_file()
{
    int iResult = 0;

    if(errorLogPath == NULL)
        goto error;

    errorLogFile = g_file_new_for_path(errorLogPath);

    errorLogStream = g_file_append_to(errorLogFile, G_FILE_CREATE_NONE, NULL, NULL);
    if(errorLogStream == NULL)
        goto error;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int error_log_set_path(const char * filePath)
{
    int iResult = 0;
    int filePathLen = 0;

    if(filePath == NULL || filePath[0] == 0)
        goto error;
    error_log_free();

    filePathLen = strlen(filePath);
    errorLogPath = (char*)malloc(sizeof(char)*(filePathLen+1));
    if(errorLogPath == NULL)
        goto error;

    strcpy(errorLogPath, filePath);

    iResult = error_log_set_file();
    goto end;

error:
    iResult = -1;
    error_log_free();
end:
    return iResult;
}

int error_log_async_write()
{
    int errorCode = 0;
    int iResult = 0;
    const char * openssl_text = NULL;

    if(errorLogStream == NULL)
        goto error;

    errorCode = getErrorCode();
    if(errorCode == 0)
    {
        iResult = -ERROR_INTERRUPT;
        goto end;
    }

    if(ERR_peek_error() > 0)
        openssl_text = ERR_error_string(ERR_get_error(), NULL);

    iResult = error_log_write(errorCode, openssl_text);
    if(iResult == -1)
        goto error;

    iResult = -errorCode;
    goto end;

error:
    iResult = 0;
end:
    return iResult;
}

void error_log_free()
{
    if(errorLogPath != NULL)
    {
        free(errorLogPath);
        errorLogPath = NULL;
    }
    if(errorLogFile != NULL)
    {
        g_object_unref(errorLogFile);
        errorLogFile = NULL;
    }
    if(errorLogStream != NULL)
    {
        g_object_unref(errorLogStream);
        errorLogStream = NULL;
    }
}
