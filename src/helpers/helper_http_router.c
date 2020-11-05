/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../../includes/helpers/helper_http_router.h"
#include "../../includes/logger.h"

// Функция преобразования текстового названия метода к enum типу
HttpRouteMethod_t string2httproutemethod(char *method) {
    if (strncmp(method, "CONNECT", 7) == 0) return HRM_CONNECT;
    if (strncmp(method, "GET", 3) == 0) return HRM_GET;
    if (strncmp(method, "DELETE", 6) == 0) return HRM_DELETE;
    if (strncmp(method, "HEAD", 4) == 0) return HRM_HEAD;
    if (strncmp(method, "OPTIONS", 7) == 0) return HRM_OPTIONS;
    if (strncmp(method, "PATCH", 5) == 0) return HRM_PATCH;
    if (strncmp(method, "POST", 4) == 0) return HRM_POST;
    if (strncmp(method, "PUT", 3) == 0) return HRM_PUT;
    if (strncmp(method, "TRACE", 5) == 0) return HRM_TRACE;
    return HRM_UNKNOWN;
}

// Функция преобразования enum типа метода к текстовому названию
char *httproutemethod2string(HttpRouteMethod_t method) {
    switch (method) {
        case HRM_CONNECT : return "CONNECT";
        case HRM_GET : return "GET";
        case HRM_DELETE : return "DELETE";
        case HRM_HEAD : return "HEAD";
        case HRM_OPTIONS : return "OPTIONS";
        case HRM_PATCH : return "PATCH";
        case HRM_POST : return "POST";
        case HRM_PUT : return "PUT";
        case HRM_TRACE : return "TRACE";
        default : return "UNKNOWN";
    }
}

// Создание структуры роутера
HttpRouter_p httprouter_create() {
    HttpRouter_p router = malloc(sizeof(HttpRouter_t));
    if (NULL == router) {
        return NULL;
    }

    router->count = 0;
    router->items = NULL;

    return router;
}

// Получение количества роутов
uint32_t httprouter_routecount(HttpRouter_p *prouter) {
    if (NULL == prouter) {
        return 0;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return 0;
    }

    return router->count;
}

// Освобождение ресурсов роутера
void httprouter_destroy(HttpRouter_p *prouter) {
    if (NULL == prouter) {
        return;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return;
    }

    while (httprouter_routeremovelast(prouter)) {
        continue;
    };

    free(router);
}

// Добавление роута
bool httprouter_routeadd(HttpRouter_p *prouter, HttpRouteMethod_t method, char *url, HttpRouteHandler_p handler) {
    if (NULL == prouter) {
        return false;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return false;
    }

    if (HRM_UNKNOWN == method) {
        return false;
    }

    HttpRoute_p newroute = malloc(sizeof(HttpRoute_t));
    if (NULL == newroute) {
        return false;
    }

    newroute->method = method;
    newroute->url = string_create("%s", url);
    newroute->handler = handler;
    newroute->prev = NULL;
    newroute->next = NULL;

    if (router->count == 0) {
        router->items = newroute;
    } else {
        HttpRoute_p tmpitem = router->items;
        while (NULL != tmpitem->next) {
            uint32_t newroute_len = strlen(newroute->url);
            uint32_t tmpitem_len = strlen(tmpitem->url);
            if ((newroute_len >= tmpitem_len) &&
                (strncmp(newroute->url, tmpitem->url, strlen(tmpitem->url)) == 0)) {

                // Если найден роут, менее детальный чем новый
                // то добавляем новый перед ним
                HttpRoute_p tmpprev = tmpitem->prev;
                if (NULL != tmpprev) {
                    newroute->prev = tmpprev;
                    tmpprev->next = newroute;
                } else {
                    router->items = newroute;
                }

                newroute->next = tmpitem;
                tmpitem->prev = newroute;
                router->count++;
                return true;
            }
            tmpitem = tmpitem->next;
        }
        newroute->prev = tmpitem;
        tmpitem->next = newroute;
    }
    router->count++;

    return true;
}

// Удаление роута
bool httprouter_routeremove(HttpRouter_p *prouter, HttpRouteMethod_t method, char *url) {
    if (NULL == prouter) {
        return false;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return false;
    }

    if ((router->count > 0) && (NULL != router->items)) {
        HttpRoute_p tmpitem = router->items;
        while (NULL != tmpitem) {
            if ((method == tmpitem->method) && (strcmp(url, tmpitem->url) == 0)) {
                HttpRoute_p prev = tmpitem->prev;
                HttpRoute_p next = tmpitem->next;

                // Линкуем список
                if ((NULL == prev) && (NULL == next)) {
                    router->count = 0;
                    router->items = NULL;
                } else if ((NULL == prev) && (NULL != next)) {
                    router->count--;
                    next->prev = NULL;
                    router->items = next;
                } else if ((NULL != prev) && (NULL == next)) {
                    router->count--;
                    prev->next = NULL;
                } else {
                    router->count--;
                    prev->next = next;
                    next->prev = prev;
                }

                // Освобождаем ресурсы, выделенные под url
                string_free(tmpitem->url);
                free(tmpitem);

                return true;
            }
            tmpitem = tmpitem->next;
        }
    }

    return true;
}

// Удаление последнего роута
bool httprouter_routeremovelast(HttpRouter_p *prouter) {
    if (NULL == prouter) {
        return false;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return false;
    }

    if ((router->count > 0) && (NULL != router->items)) {
        HttpRoute_p tmpitem = router->items;
        while (NULL != tmpitem->next) {
            tmpitem = tmpitem->next;
        }
        HttpRoute_p prev = tmpitem->prev;

        // Освобождаем ресурсы, выделенные под заголовок
        string_free(tmpitem->url);
        free(tmpitem);

        if (NULL == prev) {
            router->count = 0;
            router->items = NULL;
        } else {
            router->count--;
            prev->next = NULL;
        }
        return true;
    }
    return false;
}

// Дамп списка роутов
void httprouter_routedump(HttpRouter_p *prouter) {
    if (NULL == prouter) {
        printf("Can't dump NULL object\n");
        return;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        printf("Can't dump by pointer to NULL object\n");
        return;
    }

    printf("HttpRouter Routes Dump:\n");
    printf("====================================================================\n");
    printf ("Count: %u\n", router->count);
    if (router->count < 1) {
        printf("No routes found");
    } else {
        HttpRoute_t *tmpitem = router->items;
        while (NULL != tmpitem) {
            printf("Method: %s Url: %s\n", httproutemethod2string(tmpitem->method), tmpitem->url);
            tmpitem = tmpitem->next;
        }
    }
    printf("====================================================================\n");
}

// Роутинг запроса
bool httprouter_routerequest(HttpRouter_p *prouter, HttpRequest_p request, int client_socket) {
    // Проверяем, что указатель на роутер верный
    if (NULL == prouter) {
        return false;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return false;
    }

    // Если не заданы роуты - останавливаем обработку
    if (router->count < 0) {
        return false;
    }

    // Проверяем, что указатель на запрос верный
    if (NULL == request) {
        return false;
    }

    // Проверяем, что сокет верный
    if (client_socket < 0) {
        return false;
    }

    // Проверяем на валидность все поля запроса (для отсечения сегфолта при роутинге)
    if (NULL == request->method) {
//        printf("ERROR: request broken (1)\n");
        log_docker("[ Http ] incoming request is broken...");
        return false;
    }
    if (NULL == request->protocol) {
//        printf("ERROR: request broken (2)\n");
        log_docker("[ Http ] incoming request is broken...");
        return false;
    }
    if (NULL == request->url) {
//        printf("ERROR: request broken (3)\n");
        log_docker("[ Http ] incoming request is broken...");
        return false;
    }
    if (NULL == request->body) {
//        printf("ERROR: request broken (4)\n");
        log_docker("[ Http ] incoming request is broken...");
        return false;
    }

    // Начинаем искать нужный роут
    HttpRoute_p tmproute = router->items;

    while (NULL != tmproute) {
        if (//(NULL != tmproute->handler) &&
            (tmproute->method == string2httproutemethod(request->method)) &&
            (strncmp(tmproute->url, request->url, strlen(tmproute->url)) == 0)){
#ifdef DEBUG
            printf("====================================================================\n");
            printf("Route found:\n");
            printf("Method: %s\n", httproutemethod2string(tmproute->method));
            printf("Route: %s:\n", tmproute->url);
            printf("Request method: %s\n", request->method);
            printf("Request url: %s\n", request->url);
            printf("Handler: %s\n", (NULL == tmproute->handler ? "not assigned" : "exists" ));
            printf("====================================================================\n");
#endif
            // TODO: подумать на тему того чтобы вообще не добавлять роуты без обработчика
            // Вызываем обработчик
            if (NULL != tmproute->handler) {
                tmproute->handler(client_socket, request);
            }
            return true;
        }
        tmproute = tmproute->next;
    }

    // Если не нашли подходящего обработчика и задан дефолтный - вызываем дефолтный
    if (NULL != router->defaulthandler) {
        router->defaulthandler(client_socket, request);
        return true;
    }

    return false;
}

// Устанавливаем обработчик по-умолчанию
bool httprouter_setdefaulthandler(HttpRouter_p *prouter, HttpRouteHandler_p defaulthandler) {
    if (NULL == prouter) {
        return false;
    }
    HttpRouter_t *router = *prouter;
    if (NULL == router) {
        return false;
    }
    if (NULL != defaulthandler) {
        router->defaulthandler = defaulthandler;
        return true;
    }
    return false;
}