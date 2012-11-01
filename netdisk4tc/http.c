#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "http.h"

#define RECV_BUFFSIZE (sizeof(char) * 4096)

int headers_join(const wchar_t *key, const void *value, size_t sz, void **data) {
    char *line = NULL, *val = NULL, *pData = NULL, *nData = NULL;
    size_t size = 0, j = 0;
    val = (char *)value;
    pData = (char *)(*data);
    size = WCS_SIZEOF(key) + strlen(val) + 5;
    line = malloc(size);
    j = sprintf_s(line, size, "%ls: %s%s", key, val, HTTP_NEWLINE);
    if(pData == NULL) {
        pData = malloc(strlen(line) + 1);
        pData = _strdup(line);
    } else {
        size = strlen(pData) + strlen(line) + 1;
        nData = realloc(pData, size);
        if(pData != nData) {
            pData = nData;
        }
        nData = NULL;
        strcat_s(pData, size, line);

    }
    free(line);
    *data = pData;
    return 1;
}


HTTP_REQUEST * http_request_create(const char * path, int method, const char * body) {
    HTTP_REQUEST *request;
    request = (HTTP_REQUEST *)malloc(sizeof(HTTP_REQUEST));
    if(request == NULL) {
        return NULL;
    }
    memset(request, 0, sizeof(HTTP_REQUEST));
    request->headers = dict_initialize();
    request->method = method;
    request->version = _strdup("1.0");
    request->path = _strdup(path);
    if(body != NULL) {
        request->body = _strdup(body);
    } else {
        request->body = NULL;
    }
    return request;
}

char* http_request_tostr(const HTTP_REQUEST *request) {
    char *tmp = NULL, *headers = NULL, *ret = NULL, *nRet = NULL;
    size_t size = 0;
    size = strlen(request->path) + strlen(request->version) + 14;
    tmp = (char *)malloc(size);
    sprintf_s(tmp, size, "%s %s HTTP/%s%s", request->method == HTTP_METHOD_GET ? "GET" : "POST", request->path, request->version, HTTP_NEWLINE);
    ret = (char *)malloc(strlen(tmp) + 1);
    ret = _strdup(tmp);
    free(tmp);
    tmp = NULL;
    dict_traverse(request->headers, headers_join, &headers);
    if(headers != NULL) {
        size = strlen(ret) + strlen(headers) + 1;
        nRet = realloc(ret, size);
        if(nRet != ret) {
            ret = nRet;
        }
        nRet = NULL;
        strcat_s(ret, size, headers);
        free(headers);
        headers = NULL;
    }
    size = strlen(ret) + 3;
    nRet = realloc(ret, size);
    if(nRet != ret) {
        ret = nRet;
    }
    nRet = NULL;
    strcat_s(ret, size, HTTP_NEWLINE);
    return ret;
}

void http_request_destory(HTTP_REQUEST **request) {
    HTTP_REQUEST *req = *request;
    if(req == NULL) {
        return;
    }
    if(req->body != NULL) {
        free(req->body);
    }
    if(req->path != NULL) {
        free(req->path);
    }
    if(req->version != NULL) {
        free(req->version);
    }
    if(req->headers != NULL) {
        dict_destory(&req->headers);
    }
    free(*request);
    *request = NULL;
}

HTTP_RESPONSE * http_response_fromstr(const char * data) {
    HTTP_RESPONSE *response = NULL;
    char *line = NULL, *tmp = NULL, *sTmp = NULL;
    size_t i = 0;
    if(data == NULL) {
        return NULL;
    }
    response = malloc(sizeof(HTTP_RESPONSE));
    memset(response, 0, sizeof(HTTP_RESPONSE));
    tmp = data;
    i = strcspn(tmp, HTTP_NEWLINE);
    line = malloc(i + 1);
    strncpy(line, tmp, i);
    line[i] = '\0';
    tmp += i + strlen(HTTP_NEWLINE);
    sTmp = strtok(line, " ");
    sTmp = strtok(NULL, " ");
    response->status_code = atoi(sTmp);
    if(!response->status_code) {
        http_response_destory(response);
        free(line);
        return NULL;
    }
    sTmp = strtok(NULL, " ");
    if(sTmp == NULL) {
        http_response_destory(response);
        free(line);
        return NULL;
    }
    response->status_msg = _strdup(sTmp);
    sTmp = strtok(line, " ");
    sTmp = strtok(sTmp, "/");
    sTmp = strtok(NULL, " ");
    if(sTmp == NULL) {
        http_response_destory(response);
        free(line);
        return NULL;
    }
    response->version = _strdup(sTmp);
    free(line);
    line = NULL;
    response->headers = dict_initialize();
    while(TRUE) {
        i = strcspn(tmp, HTTP_NEWLINE);
        line = malloc(i + 1);
        strncpy(line, tmp, i);
        line[i] = '\0';


        tmp += i + strlen(HTTP_NEWLINE);
        if(strcmp(line, "") == 0) {
            free(line);
            line = NULL;
            break;
        }
        free(line);
        line = NULL;
    }
    response->body = _strdup(tmp);
    return response;
}

void http_response_destory(HTTP_RESPONSE **response) {
    HTTP_RESPONSE *rep = *response;
    if(rep == NULL) {
        return;
    }
    if(rep->body != NULL) {
        free(rep->body);
    }
    if(rep->status_msg != NULL) {
        free(rep->status_msg);
    }
    if(rep->version != NULL) {
        free(rep->version);
    }
    if(rep->headers != NULL) {
        dict_destory(&rep->headers);
    }
    free(*response);
    *response = NULL;
}

int http_connect(HTTP_CONNECTION **connection, const char *host, unsigned short port) {
	unsigned long ipaddr;
	struct hostent *hostinfo;
	HTTP_CONNECTION *conn = (HTTP_CONNECTION*)malloc(sizeof(HTTP_CONNECTION));
	if(conn == NULL) {
		return HT_MEMORY_ERROR;
	}
	memset(conn, 0, sizeof(HTTP_CONNECTION));
    conn->port = port;
	conn->address.sin_family = PF_INET;
	conn->address.sin_port = htons(port);
	if((ipaddr = inet_addr(host)) != INADDR_NONE) {
		memcpy(&conn->address.sin_addr, &ipaddr, sizeof(struct in_addr));
	} else {
		hostinfo = (struct hostent *)gethostbyname(host);
		if(hostinfo == NULL) {
			return HT_HOST_UNAVAILABLE;
		}
		memcpy(&conn->address.sin_addr, hostinfo->h_addr, 4);
	}
	conn->host = _strdup(host);
	if(conn->host == NULL) {
		http_disconnect(&conn);
		return HT_MEMORY_ERROR;
	}
	conn->socketd = socket(PF_INET, SOCK_STREAM, 0);
	if(conn->socketd == INVALID_SOCKET) {
		http_disconnect(&conn);
		return HT_RESOURCE_UNAVAILABLE;
	}
	if(connect(conn->socketd, (struct sockaddr *) &conn->address, sizeof(struct sockaddr_in)) != 0)	{
		http_disconnect(&conn);
		return HT_NETWORK_ERROR;
	}
	conn->keep_alive = TRUE;
	conn->status = HT_OK;
	*connection = conn;
	return HT_OK;
}

int http_disconnect(HTTP_CONNECTION **connection) {
	if(connection == NULL || *connection == NULL) {
		return HT_INVALID_ARGUMENT;
	}
	closesocket((*connection)->socketd);
	free((*connection)->host);
	free(*connection);
	*connection = NULL;
	return HT_OK;
}

int http_request(HTTP_CONNECTION *connection, const HTTP_REQUEST *request, HTTP_RESPONSE **response) {
	char buff[RECV_BUFFSIZE], *tmp = NULL, *nTmp = NULL;
	int ret;
    size_t size = 0, i = 0, j = 0;
    if(connection == NULL || request == NULL || response == NULL) {
        return HT_INVALID_ARGUMENT;
    }
    if(connection->status != HT_OK) {
        return HT_RESOURCE_UNAVAILABLE;
    }
    if(connection->port == 80) {
        dict_set_element(request->headers, L"Host", connection->host, strlen(connection->host) + 1);
    } else {
        size = strlen(connection->host) + 7;
        tmp = (char *)malloc(size);
        sprintf_s(tmp, size, "%s:%s", connection->host, connection->port);
        dict_set_element(request->headers, L"Host", tmp, strlen(tmp) + 1);
        free(tmp);
        tmp = NULL;
    }
    tmp = "Close";
    dict_set_element(request->headers, L"Connection", tmp, strlen(tmp) + 1);
    tmp = http_request_tostr(request);
	size = strlen(tmp) + 1;
	send(connection->socketd, tmp, size, 0);
    free(tmp);
    tmp = NULL;
    while(ret = recv(connection->socketd, buff, RECV_BUFFSIZE, 0)) {
        if(ret < RECV_BUFFSIZE) {
            buff[ret] = '\0';
        }
        if(tmp == NULL) {
            size = strlen(buff) + 1;
            tmp = (char *)malloc(size);
            tmp = _strdup(buff);
        } else {
            size = strlen(tmp) + strlen(buff) + 1;
            nTmp = realloc(tmp, size);
            if(nTmp != tmp) {
                tmp = nTmp;
                nTmp = NULL;
            }
            strcat_s(tmp, size, buff);
        }
    }
    *response = http_response_fromstr(tmp);
    if(tmp != NULL) {
        free(tmp);
    }
	return HT_OK;
}
