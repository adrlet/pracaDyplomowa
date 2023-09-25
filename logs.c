#include "log.h"

const char * logs_separator = ";";

const char * logs_get_separator()
{
    return logs_separator;
}

const char * logs_newLine = "\r\n";

const char * logs_get_newLine()
{
    return logs_newLine;
}

const char * progressText = "Wykonano: ";

const char * logs_get_progressText()
{
    return progressText;
}

const int progressTextLen = 10;

const int logs_get_progressTextLen()
{
    return progressTextLen;
}
