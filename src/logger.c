/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../includes/logger.h"
#include "../includes/env.h"

// Семафоры для исключения наложения вывода от разных потоков
pthread_mutex_t logger_stdout_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logger_stderr_mutex = PTHREAD_MUTEX_INITIALIZER;


void log_docker(char *format,...) {
    // Выставляем семафор для блокировки вывода строки лога
    pthread_mutex_lock(&logger_stdout_mutex);
 
    va_list args;
    va_start(args, format);
    // Проставляем временную метку
    time_t timestamp = (unsigned)time(NULL);
    const char *datetime_format = "%d.%m.%Y %H:%M:%S";
    struct tm lt;
    localtime_r(&timestamp, &lt);
    char *datetime_str = malloc(20);
    if (datetime_str != NULL) {
        strftime(datetime_str, 20, datetime_format, &lt);
        printf("[%s] ", datetime_str);        
        vprintf(format, args);
        printf("\n");
        va_end(args);
        fflush(stdout);
        free(datetime_str);
    }

    pthread_mutex_unlock(&logger_stdout_mutex);
}

void log_error_docker(char *format,...) {
    // Выставляем семафор для блокировки вывода строки лога
    pthread_mutex_lock(&logger_stderr_mutex);
  
    va_list args;
    va_start(args, format);
    // Проставляем временную метку
    time_t timestamp = (unsigned)time(NULL);
    const char *datetime_format = "%d.%m.%Y %H:%M:%S";
    struct tm lt;
    localtime_r(&timestamp, &lt);
    char *datetime_str = malloc(20);
    if (datetime_str != NULL) {
        strftime(datetime_str, 20, datetime_format, &lt);
        fprintf(stderr, "[%s] ", datetime_str);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
        fflush(stderr);
        free(datetime_str);
    }
    
    pthread_mutex_unlock(&logger_stderr_mutex);
}
