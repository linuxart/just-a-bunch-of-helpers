/************************************************
 
  Author: Artem Grischenko <artlinux@gmail.com>

************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../includes/helpers/helper_strings.h"
#include "../../includes/helpers/helper_http_headers.h"
#include "../../includes/helpers/helper_http_request.h"

#ifndef HELPER_HTTP_ROUTER_H
#define HELPER_HTTP_ROUTER_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum HttpRouteMethod_t {HRM_UNKNOWN, HRM_ANY, HRM_CONNECT, HRM_GET, HRM_DELETE, HRM_HEAD, HRM_OPTIONS, HRM_PATCH, HRM_POST, HRM_PUT, HRM_TRACE} HttpRouteMethod_t;

    typedef void (*HttpRouteHandler_p)(int client_socket, HttpRequest_p request);

    typedef struct HttpRoute_t {
        HttpRouteMethod_t method;
        char *url;
        HttpRouteHandler_p handler;
        struct HttpRoute_t *prev;
        struct HttpRoute_t *next;
    } HttpRoute_t;

    typedef HttpRoute_t *HttpRoute_p;

    typedef struct HttpRouter_t {
        uint32_t count;
        HttpRoute_p items;
        HttpRouteHandler_p defaulthandler;
    } HttpRouter_t;

    typedef HttpRouter_t *HttpRouter_p;

    HttpRouteMethod_t string2httproutemethod(char *method);
    char *httproutemethod2string(HttpRouteMethod_t method);

    HttpRouter_p httprouter_create();
    uint32_t httprouter_routecount(HttpRouter_p *prouter);
    void httprouter_destroy(HttpRouter_p *prouter);

    bool httprouter_routeadd(HttpRouter_p *prouter, HttpRouteMethod_t method, char *url, HttpRouteHandler_p handler);
    bool httprouter_routeremove(HttpRouter_p *prouter, HttpRouteMethod_t method, char *url);

    bool httprouter_routeremovelast(HttpRouter_p *prouter);
    void httprouter_routedump(HttpRouter_p *plist);

    bool httprouter_setdefaulthandler(HttpRouter_p *prouter, HttpRouteHandler_p defaulthandler);

    bool httprouter_routerequest(HttpRouter_p *prouter, HttpRequest_p request, int client_socket);
#ifdef __cplusplus
}
#endif

#endif /* HELPER_HTTP_ROUTER_H */

