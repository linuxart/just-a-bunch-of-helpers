/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/

#ifndef SIGNALS_H
#define SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "../includes/logger.h"

// Переменная со статусом завершения
bool terminating;

void bind_signals();

void sig_handler_int(int sig);
void sig_handler_hup(int sig);
void sig_handler_term(int sig);
void sig_handler_kill(int sig);
void sig_handler_usr1(int sig);

#ifdef __cplusplus
}
#endif

#endif /* SIGNALS_H */