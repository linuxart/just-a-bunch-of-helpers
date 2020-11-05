/* 
 * File:   src/helpers/helper_request.c
 * Author: Artem Grischenko (artlinux@gmail.com)
 */

#include "../../includes/helpers/helper_http_request.h"

HttpRequest_p httprequest_create() {
    HttpRequest_p request = malloc(sizeof(HttpRequest_t));
    request->body = NULL;
    request->method = NULL;
    request->protocol = NULL;
    request->url = NULL;
    request->headers = httpheaderslist_create();
    return request;
}

void httprequest_destroy(HttpRequest_p *prequest) {
    if (NULL == prequest) {
        return;
    }
    HttpRequest_t *request = *prequest;
    if (NULL == request) {
        return;
    }

    // Освобождаем память полей структуры
    httpheaderslist_destroy(&request->headers);
    free(request->url);
    free(request->protocol);
    free(request->method);
    free(request->body);

    // Освобождаем память структуры
    free(request);

    return;
}

bool httprequest_parse(HttpRequest_p *prequest, char *data) {
    if (NULL == prequest) {
        return false;
    }

    HttpRequest_p curr_request = *prequest;
    if (NULL == curr_request) {
        return false;
    }

    // Ищем разделитель заголовка и тела
    char *tmpbody = strstr(data, "\n\r");
    if (NULL == tmpbody) {
        return false;
    }
    curr_request->body = string_create("%s", strstr(data, "\n\r") + 2);
    uint32_t header_length = strlen(data) - strlen(curr_request->body) - 2;

    char *tmp_headers = malloc(header_length);
    strncpy(tmp_headers,data, header_length);
    tmp_headers[header_length - 1] = 0;

    // Отделяем запрос от заголовков
    char *headers;
    char *request = strtok_r(tmp_headers, "\n", &headers);

    // Разбираем запрос
    char *request_tokens;
    curr_request->method = string_create("%s", strtok_r(request, " ", &request_tokens));
    curr_request->url = string_create("%s", strtok_r(NULL, " ", &request_tokens));
    curr_request->protocol = string_create("%s", strtok_r(NULL, " ", &request_tokens));

    // Разбираем заголовки
    httpheaderslist_populate(&curr_request->headers, headers);

    // На этом этапе имеем полноценный запрос с разбиением на части
    /*printf("====================================================\n");
    printf("Request:\nMethod: %s\nUrl: %s\nProtocol: %s\n----\n", curr_request->method, curr_request->url, curr_request->protocol);
    printf("Headers:\n%s\n----\n", headers);
    printf("Body:\n%s\n", curr_request->body);
    printf("====================================================\n");*/

    free(tmp_headers);

    return true;
}

void httprequest_dump(HttpRequest_p *prequest) {
    if (NULL == prequest) {
        printf("Can't dump NULL object\n");
        return;
    }
    HttpRequest_t *request = *prequest;
    if (NULL == request) {
        printf("Can't dump by pointer to NULL object\n");
        return;
    }

    printf("HttpRequest Dump:\n");
    printf("====================================================================\n");
    printf ("Method: %s\n", request->method == NULL ? "<NULL>" : request->method );
    printf ("Protocol: %s\n", request->protocol == NULL ? "<NULL>" : request->protocol );
    printf ("Url: %s\n", request->url == NULL ? "<NULL>" : request->url );

    httpheaderslist_dump(&request->headers);

    printf("====================================================================\n");
    printf ("Body: %s\n", request->body == NULL ? "<NULL>" : request->body );    
    printf("====================================================================\n");
}