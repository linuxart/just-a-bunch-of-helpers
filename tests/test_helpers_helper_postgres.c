#include "../includes/helpers/helper_postgres.h"
#include "../includes/helpers/helper_strings.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_NORMAL "\033[0m"

uint32_t total_success = 0;
uint32_t total_failed = 0;

// Тест хелпера postgres
char *connstr = "host=localhost dbname=test user=postgres password= port=5432";

// Соединение
void test1() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);
    
    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }   

    printf ("Connect [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Проверка живости
void test2() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);
    
    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    if (!psql_ping(&connection)) {
        errors++;
    }

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }   

    printf ("Ping [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Экранирование спец символов
void test3() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    char *str1 = psql_escape_string(connection, "test\";");
    if (strcmp(str1, "'test\";'") != 0) {
        errors++;
    }
    free(str1);

    char *str2 = psql_escape_string(connection, "'test\";'");
    if (strcmp(str2, "'''test\";'''") != 0) {
        errors++;
    }
    free(str2);

    char *str3 = psql_escape_string(connection, "'");
    if (strcmp(str3, "''''") != 0) {
        errors++;
    }
    free(str3);

    char *str4 = psql_escape_string(connection, "test");
    if (strcmp(str4, "'test'") != 0) {
        errors++;
    }
    free(str4);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("EscapeString [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Экранирование спец символов
void test4() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    char *str1 = psql_escape_identifier(connection, "test\";");
    if (strcmp(str1, "\"test\"\";\"") != 0) {
        errors++;
    }
    free(str1);

    char *str2 = psql_escape_identifier(connection, "'test\";'");
    if (strcmp(str2, "\"'test\"\";'\"") != 0) {
        errors++;
    }
    free(str2);

    char *str3 = psql_escape_identifier(connection, "'");
    if (strcmp(str3, "\"'\"") != 0) {
        errors++;
    }
    free(str3);

    char *str4 = psql_escape_identifier(connection, "test");
    if (strcmp(str4, "\"test\"") != 0) {
        errors++;
    }
    free(str4);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("EscapeIdentifier [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Подписка на канал уведомлений
void test5() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    if (!psql_listen(&connection, "taskerngtest")) {
        errors++;
    }

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Listen [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Отписка от канала уведомлений
void test6() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    if (!psql_listen(&connection, "taskerngtest")) {
        errors++;
    }

    if (!psql_unlisten(&connection, "taskerngtest")) {
        errors++;
    }

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Unlisten [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Отправка уведомления в канал
void test7() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    if (!psql_notify(&connection, "taskerngtest", "test message")) {
        errors++;
    }

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Notify [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Получение уведомления из канала
void test8() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    // Генерируем рандомный канал для исключения накладок
    char *rndstr = string_random(10);
    char *channel = string_create("taskerngtest%s", rndstr);
    string_free(rndstr);

    // Подключаемся к каналу
    if (!psql_listen(&connection, channel)) {
        errors++;
    }

    // Генерируем рандомное сообщение
    char *message = string_random(32);

    // Отсылаем сообщение в канал
    if (!psql_notify(&connection, channel, message)) {
        errors++;
    }

    // Пробуем получить сообщение из канала
    char *tmpstr = psql_notification_receive(connection, channel, true);
    if (NULL != tmpstr) {
        if (strcmp(message, tmpstr) != 0) {
//            printf("NOTIFICATION\n[%s]\n[%s]\n", message, tmpstr);
            errors++;
        }
    } else {
//        printf("NOTIFICATION NULL\n");
        errors++;
    }

    // Отписываемся от канала
    if (!psql_unlisten(&connection, channel)) {
        errors++;
    }

    // Освобождаем ресурсы
    string_free(tmpstr);
    string_free(message);
    string_free(channel);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Receive Notify [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Отправка запроса
void test9() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    PGresult *res = psql_query(connection, "select 1;");

    if (!psql_query_is_success(res)) {
        errors++;
    }

    psql_free_result(res);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Execute query [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Получение количества строк
void test10() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    PGresult *res = psql_query(connection, "select 1;");

    if (!psql_query_is_success(res)) {
        errors++;
    }

    if (psql_num_rows(res) != 1) {
        errors++;
    }

    psql_free_result(res);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Num rows [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Получение количества столбцов
void test11() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }

    PGresult *res = psql_query(connection, "select 1;");

    if (!psql_query_is_success(res)) {
        errors++;
    }

    if (psql_num_cols(res) != 1) {
        errors++;
    }

    psql_free_result(res);

    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Num cols [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

// Получение значения поля
void test12() {
    uint8_t errors = 0;
    PGconn *connection = NULL;
    connection = psql_connect(connstr, false);

    if (PQstatus(connection) != CONNECTION_OK) {
        errors++;
    }
    else
    {
        PGresult *res = psql_query(connection, "select 'test string';");

        if (!psql_query_is_success(res)) {
            errors++;
        }
        else
        {
            if (psql_num_cols(res) != 1) {
                errors++;
            }
            else
            {
                char *test_val = psql_fetch_field(res, 0, 0);

                if (strcmp(test_val, "test string") != 0)
                    errors++;

                psql_free_result(res);
            }
        }
    }
    psql_disconnect(&connection);

    if (0 == errors) {
        total_success++;
    } else {
        total_failed++;
    }

    printf ("Fetch field [ %s ]\n", (0 == errors) ? COLOR_GREEN"Success"COLOR_NORMAL : COLOR_RED"Failed"COLOR_NORMAL);
}

int main() {
    printf("=========================================\n");
    printf("TestSuite: Helper Postgres\n");
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
    test12();
    printf("=========================================\n");
    printf("Success: "COLOR_GREEN"%u"COLOR_NORMAL", Failed: "COLOR_RED"%u"COLOR_NORMAL"\n", total_success, total_failed);    
    printf("=========================================\n\n\n");

    return total_failed;
}
