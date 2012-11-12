#ifndef HTTP_H__
#define HTTP_H__
#include <limits.h>
#include <winsock.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include "dictionary.h"

#ifndef TRUE
#define TRUE                        1
#endif
#ifndef FALSE
#define FALSE                       0
#endif

#define HTTP_NEWLINE                "\r\n"

#define HT_OK                       0
#define HT_FATAL_ERROR              0xFFFF
#define HT_INVALID_ARGUMENT         0xFFFE
#define HT_SERVICE_UNAVAILABLE      0xFFFD
#define HT_RESOURCE_UNAVAILABLE     0xFFFC
#define HT_MEMORY_ERROR             0xFFFB
#define HT_NETWORK_ERROR            0xFFFA
#define HT_ILLEGAL_OPERATION        0xFFF9
#define HT_HOST_UNAVAILABLE         0xFFF8        
#define HT_IO_ERROR                 0xFFF7

#define HTTP_METHOD_GET             0
#define HTTP_METHOD_POST            1

typedef struct _http_ssl{
    SSL *ssl;
    SSL_CTX *context;
    SSL_METHOD *method;
    X509 *cert;
} HTTP_SSL;

typedef struct _http_connection {
    SOCKET socketd;
    char *host;
    unsigned short port;
    struct sockaddr_in address;
    int status;
    HTTP_SSL *ssl;
} HTTP_CONNECTION;

typedef struct _http_request {
    int method;
    char *path;
    char *version;
    Dictionary *headers;
    char *body;
} HTTP_REQUEST;

typedef struct _http_response {
    char *version;
    int status_code;
    char *status_msg;
    Dictionary *headers;
    char *body;
} HTTP_RESPONSE;

int http_connect(HTTP_CONNECTION **, const char *, unsigned short, int);
int http_request(HTTP_CONNECTION *, const HTTP_REQUEST *, HTTP_RESPONSE **);
int http_disconnect(HTTP_CONNECTION **);

HTTP_REQUEST * http_request_create(const char *, int, const char *);
char * http_request_tostr(const HTTP_REQUEST *);
void http_request_destory(HTTP_REQUEST **);
HTTP_RESPONSE * http_response_fromstr(const char *);
void http_response_destory(HTTP_RESPONSE **);
#endif