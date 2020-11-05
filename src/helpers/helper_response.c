/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../../includes/helpers/helper_response.h"
#include "../../includes/const.h"
#include "../../includes/logger.h"

// Запись ответа в сокет
int response_write(int sock, char *message) {
  
    // Формируем буффер для записи в сокет и считаем его длинну
    int length = snprintf(NULL, 0, "%s", message);
    char *response = malloc(length + 1);
    if (response == NULL) {
        perror("error: can't allocate memory for write buffer");
        return -1;
    }
    snprintf(response, length + 1, "%s", message);

    socklen_t err_len;
    int error;
    err_len = sizeof(error);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &err_len);
    if (error != SO_ERROR) {
        // Пишем буфер в сокет
        int n = write(sock, response, length);
        // Проверяем, что удалось записать ответ в сокет
        if (n < 0) {
            //printf("Error: connection reset by peer\n");
            log_docker("[ Http ] connection reset by peer...");
            return -2;
        }
    }

    // Освобождаем память
    free(response);
            
    return 0;
}
