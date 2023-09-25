#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define separator logs_get_separator()
#define newLine logs_get_newLine()
#define slash '\\'

const char * logs_get_separator();
const char * logs_get_newLine();
const char * logs_get_progressText();
const int logs_get_progressTextLen();

#endif // LOG_H_INCLUDED
