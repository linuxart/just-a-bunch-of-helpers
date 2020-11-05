/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include "../../includes/signals.h"
#include "../../includes/helpers/helper_postgres.h"
#include "../../includes/helpers/helper_strings.h"

// Подключение к postgresql
PGconn *psql_connect(char *connectionstr, bool repeat_on_fail) {
    PGconn *connection = NULL;
    // Пытаемся подключится к postgres
    connection = PQconnectdb(connectionstr);

    // Проверяем что удалось подключится
    if (PQstatus(connection) == CONNECTION_OK) {
        return connection;
    } else if (repeat_on_fail) {
        // teminating - признак завершения приложения, чтобы не было бесконечного цикла
        while ((PQstatus(connection) != CONNECTION_OK) && (!terminating)) {
            if (connection != NULL) {
                PQfinish(connection);
            }
            connection = PQconnectdb(connectionstr);
            sleep(1);
        }
    }
    return connection;
}

// Закрытие соединения с postgresql
void psql_disconnect(PGconn **pconnection) {
    if (NULL == pconnection) {
        return;
    }
    PGconn *connection = (PGconn *)*pconnection;
    if (NULL == connection) {
        return;
    }
    PQfinish(connection);
}

// Проверка живости соединения
bool psql_ping(PGconn **pconnection) {
    if (NULL == pconnection) {
        return false;
    }
    PGconn *connection = (PGconn *)*pconnection;
    if (NULL == connection) {
        return false;
    }

    // Формируем запрос для пинга
    char *pingquery = string_create("select 1;");

    // Пингуем базу
    PGresult *res = NULL;
    res = PQexec(connection, pingquery);

    // Проверяем результат
    bool result = false;
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        result = true;
    } else {
        result = false;
    }    
    PQclear(res);
    res = NULL;

    // Освобождаем ресурсы
    string_free(pingquery);

    return result;
}

// Экранирование служебных символов
char *psql_escape_string(PGconn *connection, char *str) {
    if (NULL == connection) {
        return false;
    }

    char *result = PQescapeLiteral(connection, str, strlen(str));
    return result;
}

// Экранирование служебных символов
char *psql_escape_identifier(PGconn *connection, char *str) {
    if (NULL == connection) {
        return false;
    }

    char *result = PQescapeIdentifier(connection, str, strlen(str));
    return result;
}

// Подписка на канал
bool psql_listen(PGconn **pconnection, char *channel) {
    if (NULL == pconnection) {
        return false;
    }
    PGconn *connection = (PGconn *)*pconnection;
    if (NULL == connection) {
        return false;
    }

    // Формируем запрос для подписки на канал
    char *escapedchannel = psql_escape_identifier(connection, channel);
    char *listenquery = string_create("listen %s;", escapedchannel);

    // Отсылаем команду базе
    PGresult *res = NULL;
    res = PQexec(connection, listenquery);

    // Проверяем результат
    bool result = false;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        result = true;
    } else {
        result = false;
    }
    PQclear(res);
    res = NULL;

    // Освобождаем ресурсы
    string_free(listenquery);
    string_free(escapedchannel);

    return result;
}

// Отписка от канала
bool psql_unlisten(PGconn **pconnection, char *channel) {
    if (NULL == pconnection) {
        return false;
    }
    PGconn *connection = (PGconn *)*pconnection;
    if (NULL == connection) {
        return false;
    }

    // Формируем запрос для отписки от канала
    char *escapedchannel = psql_escape_identifier(connection, channel);
    char *unlistenquery = string_create("unlisten %s;", escapedchannel);

    // Отсылаем команду базе
    PGresult *res = NULL;
    res = PQexec(connection, unlistenquery);

    // Проверяем результат
    bool result = false;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        result = true;
    } else {
        result = false;
    }
    PQclear(res);
    res = NULL;

    // Освобождаем ресурсы
    string_free(unlistenquery);
    string_free(escapedchannel);

    return result;
}

// Отправка сообщения в канал
bool psql_notify(PGconn **pconnection, char *channel, char *message) {
    if (NULL == pconnection) {
        return false;
    }
    PGconn *connection = (PGconn *)*pconnection;
    if (NULL == connection) {
        return false;
    }

    // Формируем запрос для пинга
    char *escapedchannel = psql_escape_identifier(connection, channel);
    char *escapedmessage = psql_escape_string(connection, message);
    char *notifyquery = string_create("notify %s, %s;", escapedchannel, escapedmessage);

    // Пингуем базу
    PGresult *res = NULL;
    res = PQexec(connection, notifyquery);

    // Проверяем результат
    bool result = false;
    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
        result = true;
    } else {
        result = false;
    }
    PQclear(res);
    res = NULL;

    // Освобождаем ресурсы
    string_free(notifyquery);
    string_free(escapedmessage);
    string_free(escapedchannel);

    return result;
}

// Попытка получения уведомления из канала
char *psql_notification_receive(PGconn *connection, char *channel, bool nonblock) {
    // Пробуем получить данные из селектора
    PQconsumeInput(connection);
    PGnotify *notify = NULL;

    notify = PQnotifies(connection);

    char *response = NULL;
    if (nonblock) {
        if (NULL != notify) {
            response = string_create("%s", notify->extra);
            PQfreemem(notify);
        }
    }
    else
    {
        while (NULL == notify) {
            notify = PQnotifies(connection);
        }
        response = string_create("%s", notify->extra);
        PQfreemem(notify);
    }
    return response;
}

// Выполнение запроса
PGresult *psql_query(PGconn *connection, char* query) {
    return PQexec(connection, query);
}

// Получение количества строк в ответе
uint32_t psql_num_rows(PGresult *res) {
    return PQntuples(res);
}

// Получение количества колонок в ответе
uint32_t psql_num_cols(PGresult *res) {
    return PQnfields(res);
}

// Получение значения поля
char *psql_fetch_field(PGresult *res, uint32_t row, uint32_t col) {
    return PQgetvalue(res, row, col);
}

// Получение статуса запроса
bool psql_query_is_success(PGresult *res) {
    return (PQresultStatus(res) == PGRES_COMMAND_OK) || (PQresultStatus(res) == PGRES_TUPLES_OK);
}

// Освобождение ресурсов, выделенных под результат запроса
void psql_free_result(PGresult *res) {
    PQclear(res);
}