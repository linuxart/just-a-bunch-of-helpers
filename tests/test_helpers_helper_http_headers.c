#include "../includes/helpers/helper_http_headers.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_NORMAL "\033[0m"

uint32_t total_success = 0;
uint32_t total_failed = 0;

// Тест хелпера http_headers

// Создание
void test1() {
    HttpHeadersList_p headers = httpheaderslist_create();    
    if (0 == headers->count) {
        total_success++;
    } else {
        total_failed++;
    }
    printf ("Create [ %s ]\n", (0 == headers->count) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    httpheaderslist_destroy(&headers);
}

void test2() {
    uint8_t errors = 0;
    HttpHeadersList_p headers = httpheaderslist_create();    
    if (0 != headers->count) {
        errors++;
    }
    
    httpheaderslist_add(&headers, "Host", "localhost");
    
    if (1 != headers->count) {
        errors++;
    }
    
    if (errors > 0) {
        total_failed++;
    } else {
        total_success++;
    }
    
    printf ("Add [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    
    httpheaderslist_destroy(&headers);
}

void test3() {
    uint8_t errors = 0;
    HttpHeadersList_p headers = httpheaderslist_create();    
    if (0 != headers->count) {
        errors++;
    }

    static char *header_str = "Host: localhost:7000\n"
                              "User-Agent: curl/7.61.1\n"
                              "Accept: */*\n"
                              "Content-Length: 96\n"
                              "Content-Type: application/x-www-form-urlencoded\n";

    httpheaderslist_populate(&headers, header_str);    
    
    if (5 != headers->count) {
        errors++;
    }
    
    if (errors > 0) {
        total_failed++;
    } else {
        total_success++;
    }
    
    printf ("Populate [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);

    httpheaderslist_destroy(&headers);    
}

void test4() {
    uint8_t errors = 0;
    HttpHeadersList_p headers = httpheaderslist_create();
    if (0 != headers->count) {
        errors++;
    }

    static char *header_str = "Host: localhost:7000\n"
                              "User-Agent: curl/7.61.1\n"
                              "Accept: */*\n"
                              "Content-Length: 96\n"
                              "Content-Type: application/x-www-form-urlencoded\n";

    httpheaderslist_populate(&headers, header_str);

    char *tmpheader = httpheaderslist_get(&headers, "Accept");
    if ((NULL != tmpheader) && strcmp(tmpheader, "*/*") != 0 ) {
        errors++;
    }
    tmpheader = httpheaderslist_get(&headers, "User-Agent");
    if ((NULL != tmpheader) && strcmp(tmpheader, "curl/7.61.1") != 0 ) {
        errors++;
    }
    tmpheader = httpheaderslist_get(&headers, "Non-Exists-Header");
    if ((NULL != tmpheader) && strcmp(tmpheader, "non exists value") == 0 ) {
        errors++;
    }

    if (5 != headers->count) {
        errors++;
    }

    if (errors > 0) {
        total_failed++;
    } else {
        total_success++;
    }

    printf ("Get [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);

    httpheaderslist_destroy(&headers);
}

int main() {
    printf("=========================================\n");
    printf("TestSuite: Helper Http Headers\n");
    printf("Running tests...\n");
    printf("=========================================\n");
    test1();
    test2();
    test3();
    test4();
    printf("=========================================\n");
    printf("Success: "COLOR_GREEN"%u"COLOR_NORMAL", Failed: "COLOR_RED"%u"COLOR_NORMAL"\n", total_success, total_failed);    
    printf("=========================================\n\n\n");

    return total_failed;
}