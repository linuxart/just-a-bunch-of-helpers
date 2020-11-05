#include <stdio.h>
#include <strings.h>

#include "../includes/helpers/helper_http_router.h"
#include "../includes/helpers/helper_http_request.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_NORMAL "\033[0m"

uint32_t total_success = 0;
uint32_t total_failed = 0;

// Тест хелпера http_router

// Преобразование строки в метод
void test1() {
    uint8_t errors = 0;

    if (HRM_UNKNOWN != string2httproutemethod("UNKNOWNMETHOD")) errors++;
    if (HRM_UNKNOWN != string2httproutemethod("ANOTHER_UNKNOWNMETHOD")) errors++;
    if (HRM_UNKNOWN != string2httproutemethod("AND_ONCE_MORE_UNKNOWNMETHOD")) errors++;

    if (HRM_CONNECT != string2httproutemethod("CONNECT")) errors++;
    if (HRM_GET != string2httproutemethod("GET")) errors++;
    if (HRM_DELETE != string2httproutemethod("DELETE")) errors++;
    if (HRM_HEAD != string2httproutemethod("HEAD")) errors++;
    if (HRM_OPTIONS != string2httproutemethod("OPTIONS")) errors++;
    if (HRM_PATCH != string2httproutemethod("PATCH")) errors++;
    if (HRM_POST != string2httproutemethod("POST")) errors++;
    if (HRM_PUT != string2httproutemethod("PUT")) errors++;
    if (HRM_TRACE != string2httproutemethod("TRACE")) errors++;

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("string2httproutermethod [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Преобразование метода в строку
void test2() {
    uint8_t errors = 0;

    // Корректные входные данные
    if (strcmp("CONNECT", httproutemethod2string(HRM_CONNECT)) != 0) errors++;
    if (strcmp("GET", httproutemethod2string(HRM_GET)) != 0) errors++;
    if (strcmp("DELETE", httproutemethod2string(HRM_DELETE)) != 0) errors++;
    if (strcmp("HEAD", httproutemethod2string(HRM_HEAD)) != 0) errors++;
    if (strcmp("OPTIONS", httproutemethod2string(HRM_OPTIONS)) != 0) errors++;
    if (strcmp("PATCH", httproutemethod2string(HRM_PATCH)) != 0) errors++;
    if (strcmp("POST", httproutemethod2string(HRM_POST)) != 0) errors++;
    if (strcmp("PUT", httproutemethod2string(HRM_PUT)) != 0) errors++;
    if (strcmp("TRACE", httproutemethod2string(HRM_TRACE)) != 0) errors++;

    // Некорректные входные данные
    if (strcmp("UNKNOWN", httproutemethod2string(146)) != 0) errors++;
    if (strcmp("UNKNOWN", httproutemethod2string(-50)) != 0) errors++;
    if (strcmp("CONNECT", httproutemethod2string(100)) == 0) errors++;

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("httproutermethod2string [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Создание роутера
void test3() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (router->count != 0) errors++;
    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Create [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Уничтожение роутера
void test4() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (router->count != 0) errors++;
    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Destroy [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Счетчик
void test5() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Count [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Добавление роута
void test6() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", NULL);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, string2httproutemethod("INCORRECT METHOD"), "/incorrect", NULL);
    if (2 != httprouter_routecount(&router)) errors++;

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Add [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Удаление роута
void test7() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", NULL);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest2", NULL);
    if (3 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_PUT, "/puttest", NULL);
    if (4 != httprouter_routecount(&router)) errors++;
    httprouter_routeremove(&router, HRM_GET, "/gettest2");
    if (3 != httprouter_routecount(&router)) errors++;

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Remove [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Удаление роута
void test8() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", NULL);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest2", NULL);
    if (3 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_PUT, "/puttest", NULL);
    if (4 != httprouter_routecount(&router)) errors++;
    httprouter_routeremovelast(&router);
    if (3 != httprouter_routecount(&router)) errors++;

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("RemoveLast [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Роутинг
void test9() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", NULL);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest2", NULL);
    if (3 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_PUT, "/puttest", NULL);
    if (4 != httprouter_routecount(&router)) errors++;

    HttpRequest_p request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("POST");
    request->url = string_create("/posttest?test=123");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }
    httprequest_destroy(&request);

    request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("GET");
    request->url = string_create("/gettest/2");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }
    httprequest_destroy(&request);

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("RouteRequest [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Роутинг с обработчиками
bool test10_handler_executed = false;

void test10_route_handler_gettest(int client_socket, HttpRequest_p request) {
    test10_handler_executed = true;
    return;
}

void test10() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", &test10_route_handler_gettest);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest2", NULL);
    if (3 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_PUT, "/puttest", NULL);
    if (4 != httprouter_routecount(&router)) errors++;

    HttpRequest_p request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("POST");
    request->url = string_create("/posttest?test=123");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }
    httprequest_destroy(&request);

    request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("GET");
    request->url = string_create("/gettest/2");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }

    if (!test10_handler_executed)
        errors++;

    httprequest_destroy(&request);

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("RouteRequestHandler [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Роутинг с вызовом дефолтного обработчиками
bool test11_default_handler_executed = false;

void test11_route_default_handler(int client_socket, HttpRequest_p request) {
    test11_default_handler_executed = true;
    return;
}

void test11() {
    uint8_t errors = 0;

    HttpRouter_p router = httprouter_create();
    if (0 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest", &test10_route_handler_gettest);
    if (1 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_POST, "/posttest", NULL);
    if (2 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_GET, "/gettest2", NULL);
    if (3 != httprouter_routecount(&router)) errors++;
    httprouter_routeadd(&router, HRM_PUT, "/puttest", NULL);
    if (4 != httprouter_routecount(&router)) errors++;

    httprouter_setdefaulthandler(&router, &test11_route_default_handler);

    HttpRequest_p request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("POST");
    request->url = string_create("/posttest?test=123");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }
    httprequest_destroy(&request);

    request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("GET");
    request->url = string_create("/random/non/exists/method");
    request->body = string_create("{'test' : 'true'}");
    if (!httprouter_routerequest(&router, request, 1)) {
        errors++;
    }

    if (!test11_default_handler_executed)
        errors++;

    httprequest_destroy(&request);

    httprouter_destroy(&router);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("RouteRequestHandlerDefault [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

int main() {
    printf("=========================================\n");
    printf("TestSuite: Helper Router\n");
    printf("Running tests...\n");
    printf("=========================================\n");
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    printf("=========================================\n");
    printf("Success: "COLOR_GREEN"%u"COLOR_NORMAL", Failed: "COLOR_RED"%u"COLOR_NORMAL"\n", total_success, total_failed);    
    printf("=========================================\n\n\n");

    return total_failed;
}