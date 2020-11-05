/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../../includes/helpers/helper_strings.h"
#include <time.h>

// Создание строки
char *string_create(char *format,...) {
    va_list args;
    // Считаем необходимый объем памяти
    va_start(args, format);    
    uint32_t length = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char *output = malloc(length + 1);
    if (output != NULL){
        va_start(args, format);
        vsnprintf(output, length + 1, format, args);
        va_end(args);
    }
    return output;
}

// Освобождение строки
void string_free(char *str) {
    if (str != NULL) {
        free(str);
    }
}

// Обрезка пробелов в начале строки
char *string_trim(char * s) {
    int l = strlen(s);

    while(isspace(s[l - 1])) --l;
    while(* s && isspace(* s)) ++s, --l;

    return strndup(s, l);
}

// Генерация случайной цифробуквенной строки
char *string_random(uint16_t size) {
    size++;
    char *str = malloc(size + 1);
    if (NULL == str) {
        return NULL;
    }

    char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint16_t stringlen = strlen(charset);

    // Инициируем генератор случайных чисел
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);

    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % stringlen;
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

// Замена символа в строке
int string_replace_char(char *str, char found, char replace) {
    uint32_t replaces = 0;
    char *pos = strchr(str, found);
    while (NULL != pos) {
        *pos = replace;
        replaces++;
        pos = strchr(str, found);
    }
    return replaces;
}