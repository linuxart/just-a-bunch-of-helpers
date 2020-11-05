/* 
 * File:   includes/helpers/helper_response.h
 * Author: Artem Grischenko (artlinux@gmail.com)
 */

#ifndef RESPONSE_HANDLERS_H
#define RESPONSE_HANDLERS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/epoll.h> 
    
    
int response_write(int sock, char *message);


#ifdef __cplusplus
}
#endif

#endif /* RESPONSE_HANDLERS_H */

