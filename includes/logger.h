/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#ifndef LOGGER_H
#define LOGGER_H

    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <pthread.h>


    void log_docker(char *format,...);
    void log_error_docker(char *format,...);

#endif /* LOGGER_H */