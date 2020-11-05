#include "../includes/helpers/helper_http_request.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_NORMAL "\033[0m"

uint32_t total_success = 0;
uint32_t total_failed = 0;

// Тест хелпера http_request

// Создание
void test1() {
    HttpRequest_p request = httprequest_create();

    if (NULL != request) {
        total_success++;
    } else {
        total_failed++;
    }
    printf ("Create [ %s ]\n", (NULL != request) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    httprequest_destroy(&request);
}

// Уничтожение
void test2() {
    HttpRequest_p request = httprequest_create();
    request->protocol = string_create("HTTP/1.1");
    request->method = string_create("POST");
    request->url = string_create("/posttest?test=123");
    request->body = string_create("{'test' : 'true'}");

    if (NULL != request) {
        total_success++;
    } else {
        total_failed++;
    }
    printf ("Destroy [ %s ]\n", (NULL != request) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    httprequest_destroy(&request);
}

// Парсинг запроса
void test3() {
    uint8_t errors = 0;

    char *requeststring = "POST /test HTTP/1.1\n"
                          "Host: localhost:7000\n"
                          "User-Agent: curl/7.61.1\n"
                          "Accept: */*\n"
                          "Content-Length: 96\n"
                          "Content-Type: application/x-www-form-urlencoded\n"
                          "\n\r"
                          "{\"total\":78,\"paid\":49,\"from\":\"2018-10-25 17:00:00\",\"to\":\"2018-10-25 18:00:00\",\"conversion\":0.63}\n";

    HttpRequest_p request = httprequest_create();
    if (!httprequest_parse(&request, requeststring)) {
        errors++;
    }
    if (strcmp(request->method, "POST") != 0) {
        errors++;
    }
    if (strcmp(request->protocol, "HTTP/1.1") != 0) {
        errors++;
    }
    if (strcmp(request->url, "/test") != 0) {
        errors++;
    }
    char *tmpheader = httpheaderslist_get(&request->headers, "Host");
    if ((NULL != tmpheader) && strcmp(tmpheader, "localhost:7000") != 0) {
        errors++;
    }
    tmpheader = httpheaderslist_get(&request->headers, "Accept");
    if ((NULL != tmpheader) && strcmp(tmpheader, "*/*") != 0) {
        errors++;
    }
    tmpheader = httpheaderslist_get(&request->headers, "Content-Length");
    if ((NULL != tmpheader) && strcmp(tmpheader, "96") != 0) {
        errors++;
    }
    tmpheader = httpheaderslist_get(&request->headers, "Non-exists-header");
    if ((NULL != tmpheader) && strcmp(tmpheader, "non exists value") == 0) {
        errors++;
    }

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }
    printf ("Parse [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    httprequest_destroy(&request);
}


int main() {
    printf("=========================================\n");
    printf("TestSuite: Helper Http Request\n");
    printf("Running tests...\n");
    printf("=========================================\n");
    test1();
    test2();
    //test3();
    printf("=========================================\n");
    printf("Success: "COLOR_GREEN"%u"COLOR_NORMAL", Failed: "COLOR_RED"%u"COLOR_NORMAL"\n", total_success, total_failed);    
    printf("=========================================\n\n\n");

    return total_failed;
}