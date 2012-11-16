#include "wrapper.h"
#include "http.h"

int wp_http(lua_State * l) {
    char *tmp = NULL, *sTmp = NULL, *pTmp = NULL;
    char *host = NULL, *path = NULL, *body = NULL;
    HTTP_CONNECTION *conn;
    HTTP_REQUEST *request;
    HTTP_RESPONSE *response;
    int i = 0, method = 0, port = 0, isssl = 0;
    if(lua_gettop(l) != 1 || !lua_istable(l, -1)) {
        lua_pushnil(l);
        OutputDebugString("arguments error");
        return 1;
    }
    lua_pushnil(l);
    while (lua_next(l, -2)) {
        tmp = (char *)lua_tostring(l, -2);
        sTmp = (char *)lua_tostring(l, -1);
        if(strcmp(tmp, "url") == 0) {
            if(*(sTmp + 4) == 's') {
                isssl = 1;
                port = 443;
                pTmp = sTmp + 8;
            } else {
                pTmp = sTmp + 7;
                port = 80;
            }
            while(*(pTmp + i) != ':' && *(pTmp + i) != '/' && *(pTmp + i) != '\0') {
                i++;
            }
            host = (char *) calloc(i + 1, sizeof(char));
            strlcpy(host, pTmp, i + 1);
            pTmp = pTmp + i;
            path = _strdup(pTmp);
        }
        if(strcmp(tmp, "method") == 0) {
            if(strcmp(sTmp, "post") == 0) {
                method = HTTP_METHOD_POST;
            } else {
                method = HTTP_METHOD_GET;
            }
        }
        if(strcmp(tmp, "body") == 0) {
            body = _strdup(sTmp);
        }
        lua_pop(l, 1);
    }
    request = http_request_create(path, method, body);
    i = 0;
    i = http_connect(&conn, host, port, isssl);
    if(body != NULL) {
        free(body);
        body = NULL;
    }
    if(host != NULL) {
        free(host);
        host = NULL;
    }
    if(path != NULL) {
        free(path);
        path = NULL;
    }
    if(i != HT_OK) {
        lua_pushnil(l);
        return 1;
    }
    if(http_request(conn, request, &response) != HT_OK) {
        http_disconnect(&conn);
        lua_pushnil(l);
        return 1;
    }
    lua_pushstring(l, response->body);
    http_response_destory(&response);
    http_disconnect(&conn);
    return 1;
}

