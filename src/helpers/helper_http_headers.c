/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../../includes/helpers/helper_http_headers.h"

// Создание списка заголовков
HttpHeadersList_p httpheaderslist_create() {
    HttpHeadersList_p headerslist = malloc(sizeof(HttpHeadersList_t));
    if (NULL == headerslist) {
        return NULL;
    }
    
    headerslist->count = 0;
    headerslist->items = NULL;
    
    return headerslist;
}

// Получение количества заголовков
uint32_t httpheaderslist_count(HttpHeadersList_p *plist) {
    if (NULL == plist) {
        return 0;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return 0;
    }
    
    return list->count;
}

// Добавление заголовка
bool httpheaderslist_add(HttpHeadersList_p *plist, char *name, char *headervalue) {
    if (NULL == plist) {
        return false;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return false;
    }

    HttpHeader_p newheader = malloc(sizeof(HttpHeader_t));
    if (NULL == newheader) {
        return false;
    }

    newheader->name = string_create("%s", name);
    newheader->headervalue = string_create("%s", headervalue);
    newheader->prev = NULL;
    newheader->next = NULL;

    if (list->count == 0) {
        list->items = newheader;
    } else {
        HttpHeader_p tmpitem = list->items;
        while (NULL != tmpitem->next) {
            tmpitem = tmpitem->next;
        }
        newheader->prev = tmpitem;
        tmpitem->next = newheader;
    }
    list->count++;

    return true;
}

// Добавление заголовка блоком
bool httpheaderslist_populate(HttpHeadersList_p *plist, char *headers) {
    char *source = string_create("%s", headers);
    char *tmp_header_str;
    char *curr_header_str;

    // Бьем блок заголовков на отдельные заголовки
    curr_header_str = strtok_r(source, "\n", &tmp_header_str);
    while (NULL != curr_header_str) {
        
        // Бьем строку на имя заголовка и его значение
        char *header_value;
        char *header_name = strtok_r(curr_header_str, ":", &header_value);
        char *header_name_trimmed = string_trim(header_name);
        char *header_value_trimmed = string_trim(header_value);

        // Добавляем разобранный заголовок в список
        httpheaderslist_add(plist, header_name_trimmed, header_value_trimmed);

        free(header_name_trimmed);
        free(header_value_trimmed);

        curr_header_str = strtok_r(NULL, "\n", &tmp_header_str);
    }
    free(source);
    return true;
}

// Получение значения заголовка по имени
char *httpheaderslist_get(HttpHeadersList_p *plist, char *name) {
    if (NULL == plist) {
        return NULL;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return NULL;
    }

    if (list->count > 0) {
        HttpHeader_p tmpitem = list->items;
        while (NULL != tmpitem) {
            if (strcmp(name, tmpitem->name) == 0) {
                return tmpitem->headervalue;
            }
            tmpitem = tmpitem->next;
        }
    }    

    return NULL;
}

// Удаление заголовка по имени
bool httpheaderslist_remove(HttpHeadersList_p *plist, char *name) {
    if (NULL == plist) {
        return false;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return false;
    }

    if ((list->count > 0) && (NULL != list->items)) {
        HttpHeader_p tmpitem = list->items;
        while (NULL != tmpitem) {
            if (strcmp(name, tmpitem->name) == 0) {
                HttpHeader_p prev = tmpitem->prev;
                HttpHeader_p next = tmpitem->next;

                // Линкуем список
                if ((NULL == prev) && (NULL == next)) {
                    list->count = 0;
                    list->items = NULL;
                } else if ((NULL == prev) && (NULL != next)) {
                    list->count--;
                    next->prev = NULL;
                    list->items = next;
                } else if ((NULL != prev) && (NULL == next)) {
                    list->count--;
                    prev->next = NULL;
                } else {
                    list->count--;
                    prev->next = next;
                    next->prev = prev;
                }

                // Освобождаем ресурсы, выделенные под заголовок
                string_free(tmpitem->name);
                string_free(tmpitem->headervalue);
                free(tmpitem);

                return true;
            }
            tmpitem = tmpitem->next;
        }
    }

    return true;
}

// Удаление последнего заголовка (для очистки списка)
bool httpheaderslist_remove_last(HttpHeadersList_p *plist) {
    if (NULL == plist) {
        return false;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return false;
    }

    if ((list->count > 0) && (NULL != list->items)) {
        HttpHeader_p tmpitem = list->items;
        while (NULL != tmpitem->next) {
            tmpitem = tmpitem->next;
        }
        HttpHeader_p prev = tmpitem->prev;

        // Освобождаем ресурсы, выделенные под заголовок
        string_free(tmpitem->name);
        string_free(tmpitem->headervalue);
        free(tmpitem);

        if (NULL == prev) {
            list->count = 0;
            list->items = NULL;
        } else {
            list->count--;
            prev->next = NULL;
        }
        return true;
    }
    return false;
}

// Освобождение ресурсов списка
void httpheaderslist_destroy(HttpHeadersList_p *plist) {
    if (NULL == plist) {
        return;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        return;
    }

    while (httpheaderslist_remove_last(plist)) {
        continue;
    };

    free(list);
    list = NULL;
}


void httpheaderslist_dump(HttpHeadersList_p *plist) {
    if (NULL == plist) {
        printf("Can't dump NULL object\n");
        return;
    }
    HttpHeadersList_t *list = *plist;
    if (NULL == list) {
        printf("Can't dump by pointer to NULL object\n");
        return;
    }

    printf("HttpHeadersList Dump:\n");
    printf("====================================================================\n");
    printf ("Count: %u\n", list->count);
    if (list->count < 1) {
        printf("No headers found\n");
    } else {
        HttpHeader_t *tmpitem = list->items;
        while (NULL != tmpitem) {
            printf("Header: %s Value: %s\n", tmpitem->name, tmpitem->headervalue);
            tmpitem = tmpitem->next;
        }
    }
    printf("====================================================================\n");
}