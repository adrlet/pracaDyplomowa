#ifndef ERRORLOG_H_INCLUDED
#define ERRORLOG_H_INCLUDED

int error_log_async_write();

int error_log_set_path(const char * filePath);

void error_log_free();

#endif // ERRORLOG_H_INCLUDED
