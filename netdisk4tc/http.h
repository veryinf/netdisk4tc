#ifndef HTTP_H__
#define HTTP_H__
#include <limits.h>
#include <winsock.h>

typedef struct http_connection {
    SOCKET socketd;
    int status;
    char *host;
    struct sockaddr_in address;
    int persistent;
    HTTP_AUTH_INFO *auth_info;
} HTTP_CONNECTION;

typedef struct http_request {
    int method;
    char *resource;
    HTTP_HEADER_FIELD *first_header_field;
    HTTP_HEADER_FIELD *last_header_field;
    HTTP_STORAGE *content;
} HTTP_REQUEST;

typedef struct http_response {
    int status_code;
    char *status_msg;
    char *version;
    HTTP_HEADER_FIELD *first_header_field;
    HTTP_HEADER_FIELD *last_header_field;
    HTTP_STORAGE *content;
} HTTP_RESPONSE;

#endif