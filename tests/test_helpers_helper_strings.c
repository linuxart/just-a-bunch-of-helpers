#include "../includes/helpers/helper_strings.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_NORMAL "\033[0m"

uint32_t total_success = 0;
uint32_t total_failed = 0;

// Тест хелпера http_headers

// Создание
void test1() {
    uint8_t errors = 0;
    char *str = string_create("%s", "test string");
    if (strcmp(str, "test string") != 0) errors++;
    string_free(str);
    
    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }   

    printf ("Create [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Удаление пробелов по краям строки
void test2() {
    uint8_t errors = 0;
    char *str = string_create("%s", "  test string ");
    char *trimmed_str = string_trim(str);
    if (strcmp(trimmed_str, "test string") != 0) errors++;
    
    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }   

    free(trimmed_str);
    free(str);    
    
    printf ("Trim [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Случайная строка
void test3() {
    uint8_t errors = 0;
    char *str1 = string_random(20);
    char *str2 = string_random(20);

    if (strlen(str1) != 20) {
        errors++;
    }
    if (strlen(str2) != 20) {
        errors++;
    }

    // Проверяем что две сгенерированные подряд строки не равны
    if (strcmp(str1, str2) == 0) {
        errors++;
    }

    free(str1);
    free(str2);

    // Прогон генерации длинны строки от 1 до 1000
    for (int i = 1; i < 1000; i++) {
        char *tmpstr = string_random(i);
        if (strlen(tmpstr) != i) {
            errors++;
        }
        string_free(tmpstr);
    }    

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Random [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Замена символа в строке
void test4() {
    uint8_t errors = 0;
    char *str1 = string_create("test/string/to/replace/char");
    char *str2 = string_create("test_string_to_replace_char");

    int replaces = string_replace_char(str1, '/', '_');
    if (replaces != 4) {
        errors++;
    }

    // Проверяем что две сгенерированные подряд строки не равны
    if (strcmp(str1, str2) != 0) {
        errors++;
    }

    string_free(str2);
    string_free(str1);

    printf ("Char replace [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

/*
void test2() {
    uint8_t errors = 0;
    HttpHeadersList_p headers = httpheaderslist_create();    
    if (0 != headers->count) {
        errors++;
    }
    
    httpheaderslist_add(&headers, "Host", "localhost:7000");
    
    if (1 != headers->count) {
        errors++;
    }
    
    printf ("Add [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
    
    httpheaderslist_destroy(&headers);
}
*/
int main() {
    printf("=========================================\n");
    printf("TestSuite: Helper Strings\n");
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