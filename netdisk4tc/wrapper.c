#include <ocidl.h>
#include "utility.h"
#include "wrapper.h"
#include "http.h"
#include "resource.h"
#include "ndplugin.h"
#include "ui.h"
#include "webbrowser/cwebpage.h"
#include "disk.h"

int wp_utoc(lua_State *l) {
    char *tmp = NULL;
    if(lua_gettop(l) != 1 || !lua_isstring(l, -1)) {
        lua_pushnil(l);
        OutputDebugString("arguments error");
        return 1;
    }
    tmp = lua_tostring(l, -1);
    lua_pop(l, 1);
    tmp = convert(tmp, CP_UTF8, 936);
    lua_pushstring(l, tmp);
    free(tmp);
    tmp = NULL;
    return 1;
}

int wp_savt(lua_State *l) {
    char *tmp = NULL;
    wchar_t *key = NULL, *token = NULL;
    NDisk *disk = NULL;
    if(lua_gettop(l) != 2 || !lua_isstring(l, 1) || !lua_isstring(l, 2)) {
        lua_pushboolean(l, FALSE);
        OutputDebugString("arguments error");
        return 1;
    }
    key = ctow(lua_tostring(l, 1));
    token = ctow(lua_tostring(l, 2));
    if(dict_get_element_s(available_disks, key, (void **)&disk) == DICT_OK) {
        if(disk->token) {
            free(disk->token);
        }
        disk->token = token;
        token = NULL;
        lua_pushboolean(l, TRUE);
        ndisks_save();
    } else {
        lua_pushboolean(l, FALSE);
    }
    free(key);
    key = NULL;
    if(token) {
        free(token);
        token = NULL;
    }
    return 1;
}

int wp_explorer(lua_State *l) {
    char *tmp = NULL;
    wchar_t *sTmp = NULL;
    MSG	msg;
    WNDCLASSEX wc;
    RECT rect;
    int width = 500, height = 600;
    HWND hParent = GetActiveWindow();
    if(lua_gettop(l) != 1 || !lua_istable(l, -1)) {
        lua_pushnil(l);
        OutputDebugString("arguments error");
        return 1;
    }
    lua_getfield(l, -1, "width");
    if(lua_isnumber(l, -1)) {
        width = lua_tointeger(l, -1);
        lua_pop(l, 1);
    }
    lua_getfield(l, -1, "height");
    if(lua_isnumber(l, -1)) {
        height = lua_tointeger(l, -1);
        lua_pop(l, 1);
    }
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInst;
    wc.lpfnWndProc = AuthProc;
    wc.lpszClassName = L"WebBrowser";
    RegisterClassEx(&wc);
    ZeroMemory(&msg, sizeof(MSG));
    EnableWindow(hParent, FALSE);
    GetWindowRect(hParent, &rect);
    lua_getfield(l, -1, "title");
    if(lua_isstring(l, -1)) {
        tmp = lua_tostring(l, -1);
        sTmp = ctow(tmp);
        lua_pop(l, 1);
    }
    msg.hwnd = CreateWindowEx(0, L"WebBrowser", sTmp ? sTmp :L"ÑéÖ¤", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (rect.right - rect.left - width) / 2, (rect.bottom - rect.top - height) / 2, width, height, hParent, NULL, hInst, 0);
    if(sTmp) {
        free(sTmp);
        sTmp = NULL;
    }
    lua_getfield(l, -1, "url");
    if(lua_isstring(l, -1)) {
        tmp = lua_tostring(l, -1);
        sTmp = ctow(tmp);
    }
    DisplayHTMLPage(msg.hwnd, sTmp);
    free(sTmp);
    sTmp = NULL;
    ShowWindow(msg.hwnd, SW_SHOWNORMAL);
    UpdateWindow(msg.hwnd);

    tmp = NULL;
    while (GetMessage(&msg, 0, 0, 0) == 1) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if(msg.message == WM_USER_AUTHTOKENDATA) {
            tmp = wtoc((char *)msg.wParam);
        }
    }
    EnableWindow(hParent, TRUE);
    SetForegroundWindow(hParent);
    if(tmp) {
        lua_pushstring(l, tmp);
        free(tmp);
        tmp = NULL;
    } else {
        lua_pushnil(l);
    }
    return 1;
}

int wp_http(lua_State *l) {
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

