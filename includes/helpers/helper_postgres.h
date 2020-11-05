/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#ifndef HELPER_POSTGRES_H
#define HELPER_POSTGRES_H

#include <stdbool.h>
#include <stdint.h>

#include <openssl/opensslconf.h>
#include <libpq-fe.h>

#ifdef __cplusplus
extern "C" {
#endif

    PGconn *psql_connect(char *connectionstr, bool repeat_on_fail);
    void psql_disconnect(PGconn **pconnection);
    bool psql_ping(PGconn **pconnection);
    char *psql_escape_string(PGconn *connection, char *str);
    char *psql_escape_identifier(PGconn *connection, char *str);
    bool psql_listen(PGconn **pconnection, char *channel);
    bool psql_unlisten(PGconn **pconnection, char *channel);
    bool psql_notify(PGconn **pconnection, char *channel, char *message);
    char *psql_notification_receive(PGconn *connection, char *channel, bool nonblock);
    PGresult *psql_query(PGconn *connection, char* query);
    uint32_t psql_num_rows(PGresult *res);
    uint32_t psql_num_cols(PGresult *res);
    char *psql_fetch_field(PGresult *res, uint32_t row, uint32_t col);
    bool psql_query_is_success(PGresult *res);
    void psql_free_result(PGresult *res);

#ifdef __cplusplus
}
#endif

#endif /* HELPER_POSTGRES_H */