#include <string.h>
#include <gtk/gtk.h>

#include "jobsLog.h"
#include "jobsDialog.h"
#include "log.h"

char * jobsLogPath = NULL;
GFile * jobsLogFile = NULL;
GFileOutputStream * jobsLogStream = NULL;

const char * results[] =
{
    "Success",
    "Failed",
    "Interrupted"
};

static int jobs_log_write(const char * operation, unsigned char result)
{
    static GMutex jobs_mutex;

    int iResult = 0;
    static const char * newline = "\r\n";
    GDateTime * jobsDateTime = NULL;
    char * jobsDateTimeString = NULL;

    char * totalBuffer = NULL;
    int totalBufferLength = 0;

    jobsDateTime = g_date_time_new_now_local();
    jobsDateTimeString = g_date_time_format(jobsDateTime, "%d/%m/%Y %H:%M:%S");

    totalBufferLength = strlen(jobsDateTimeString)+strlen(operation)+strlen(results[result])+strlen(newline)+2;

    totalBuffer = (char*)malloc(sizeof(char)*(totalBufferLength+1));
    totalBuffer[0] = 0;
    strcat(totalBuffer, jobsDateTimeString);
    strcat(totalBuffer, separator);
    strcat(totalBuffer, operation);
    strcat(totalBuffer, separator);
    strcat(totalBuffer, results[result]);
    strcat(totalBuffer, newline);

    g_mutex_lock (&jobs_mutex);
    iResult = g_output_stream_write(G_OUTPUT_STREAM(jobsLogStream), totalBuffer, totalBufferLength, NULL, NULL);
    if(iResult == -1)
        goto error;

    jobs_dialog_append_string(totalBuffer);

    iResult = 0;
    goto end;

error:
    iResult = -1;

end:
    g_mutex_unlock (&jobs_mutex);
    if(jobsDateTimeString != NULL)
        g_free(jobsDateTimeString);
    if(totalBuffer != NULL)
        free(totalBuffer);

    return iResult;
}

static int jobs_log_set_file()
{
    int iResult = 0;

    if(jobsLogPath == NULL)
        goto error;

    jobsLogFile = g_file_new_for_path(jobsLogPath);

    jobsLogStream = g_file_append_to(jobsLogFile, G_FILE_CREATE_NONE, NULL, NULL);
    if(jobsLogStream == NULL)
        goto error;

    goto end;

error:
    iResult = -1;
    jobs_log_free();
end:
    return iResult;
}

int jobs_log_set_path(const char * filePath)
{
    int iResult = 0;
    int filePathLen = 0;

    if(filePath == NULL || filePath[0] == 0)
        goto error;

    jobs_log_free();

    filePathLen = strlen(filePath);
    jobsLogPath = (char*)malloc(sizeof(char)*(filePathLen+1));
    if(jobsLogPath == NULL)
        goto error;

    strcpy(jobsLogPath, filePath);

    iResult = jobs_log_set_file();
    goto end;

error:
    iResult = -1;
    jobs_log_free();
end:
    return iResult;
}

int jobs_log_async_write(const char * operation, unsigned char result)
{
    int iResult = 0;

    if(operation == NULL || result >= JOBS_RESULTS_NUM)
        goto error;

    if(jobsLogStream == NULL)
        goto error;

    iResult = jobs_log_write(operation, result);
    if(iResult == -1)
        goto error;

    iResult = 0;
    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

void jobs_log_free()
{
    if(jobsLogPath != NULL)
    {
        free(jobsLogPath);
        jobsLogPath = NULL;
    }
    if(jobsLogFile != NULL)
    {
        g_object_unref(jobsLogFile);
        jobsLogFile = NULL;
    }
    if(jobsLogStream != NULL)
    {
        g_object_unref(jobsLogStream);
        jobsLogStream = NULL;
    }
}
