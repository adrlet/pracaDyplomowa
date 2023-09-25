#ifndef JOBSLOG_H_INCLUDED
#define JOBSLOG_H_INCLUDED

#define JOBS_RESULTS_NUM 3

int jobs_log_set_path(const char * filePath);

int jobs_log_async_write(const char * operation, unsigned char result);

void jobs_log_free();

#endif // JOBSLOG_H_INCLUDED
