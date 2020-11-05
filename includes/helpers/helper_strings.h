/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#ifndef HELPER_STRINGS_H
#define HELPER_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
    
char *string_create(char *format,...);
void string_free(char * str);
char *string_trim(char * s);
char *string_random(uint16_t size);
int string_replace_char(char *str, char found, char replace);

#ifdef __cplusplus
}
#endif

#endif /* HELPER_STRINGS_H */

