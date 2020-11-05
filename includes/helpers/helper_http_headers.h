/* 
 * File:   includes/helpers/helper_http_headers.h
 * Author: Artem Grischenko (artlinux@gmail.com)
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <malloc.h>

#include "../../includes/helpers/helper_strings.h"

#ifndef HELPER_HEADERS_H
#define HELPER_HEADERS_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct HttpHeader_t {
        char *name;
        char *headervalue;
        struct HttpHeader_t *prev;
        struct HttpHeader_t *next;
    } HttpHeader_t;
    
    typedef HttpHeader_t *HttpHeader_p;

    typedef struct HttpHeadersList_t {
        uint32_t count;
        HttpHeader_p items;    
    } HttpHeadersList_t;
    
    typedef HttpHeadersList_t *HttpHeadersList_p;
    
    HttpHeadersList_p httpheaderslist_create();
    void httpheaderslist_destroy(HttpHeadersList_p *plist);
    uint32_t httpheaderslist_count(HttpHeadersList_p *plist);
    bool httpheaderslist_add(HttpHeadersList_p *plist, char *name, char *value);
    bool httpheaderslist_populate(HttpHeadersList_p *plist, char *headers);
    char *httpheaderslist_get(HttpHeadersList_p *plist, char *name);
    bool httpheaderslist_remove(HttpHeadersList_p *plist, char *name);
    bool httpheaderslist_remove_last(HttpHeadersList_p *plist);
    void httpheaderslist_dump(HttpHeadersList_p *plist);
    
    
#ifdef __cplusplus
}
#endif

#endif /* HELPER_HEADERS_H */

