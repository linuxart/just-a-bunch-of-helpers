/* 
 * File:   includes/helpers/helper_http_request.h
 * Author: Artem Grischenko (artlinux@gmail.com)
 */

#include "../../includes/helpers/helper_http_headers.h"

#ifndef HELPER_REQUEST_H
#define HELPER_REQUEST_H

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct HttpRequest_t {
    char *method;
    char *url;
    char *protocol;    
    HttpHeadersList_p headers;
    char *body;
} HttpRequest_t;

typedef HttpRequest_t *HttpRequest_p;

HttpRequest_p httprequest_create();
void httprequest_destroy(HttpRequest_p *prequest);
bool httprequest_parse(HttpRequest_p *prequest, char *data);
void httprequest_dump(HttpRequest_p *prequest);

#ifdef __cplusplus
}
#endif

#endif /* HELPER_REQUEST_H */

