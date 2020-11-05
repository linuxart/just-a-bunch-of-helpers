/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../includes/signals.h"

// Обработчики сигналов

void sig_handler_int(int sig)
{
    log_docker("[ Main ] received interrupt signal");
    terminating = true;
    //raise(SIGUSR1);
    
}

void sig_handler_hup(int sig)
{
    log_docker("[ Main ] ignoring SIGHUP");
}

void sig_handler_term(int sig)
{
    log_docker("[ Main ] received terminate signal");
    terminating = true;
}

void sig_handler_pipe(int sig)
{
    // Игнорируем сигнал при обрыве соединения
    //log_docker("[ Main ] ignoring SIGPIPE");
}


void bind_signals() {
    // Ставим обработчики сигналов
    signal(SIGHUP, sig_handler_hup);
    signal(SIGTERM, sig_handler_term);
    signal(SIGINT, sig_handler_int);
    signal(SIGPIPE, sig_handler_pipe);
}