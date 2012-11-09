#include <openssl/ssl.h>
#include "ui.h"
#include "resource.h"
#include "disk.h"
#include "http.h"
#include "ndplugin.h"

#define INPUT_MAX 1024

INT_PTR CALLBACK NewDiskDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
    WCHAR buff[INPUT_MAX];
    WCHAR *tmp;
    NDISK *disk = NULL;
    int i = 0;
    //HTTP_CONNECTION *conn;
    //HTTP_REQUEST *request;
    //HTTP_RESPONSE *response;
    //lua_State *L;
    switch (Message) {
        case WM_INITDIALOG:
            for(i = NDISK_FIRST_DISK; i <= NDISK_LAST_DISK; i++) {
                SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)available_disk_types[i].description);
            }
            SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_SETCURSEL, 0, (LPARAM)NULL);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_BUTTON_REGISTER:
                    GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, (LPWSTR)&buff, INPUT_MAX);
                    for(i = NDISK_FIRST_DISK; i <= NDISK_LAST_DISK; i++) {
                        if(wcscmp(buff, available_disk_types[i].description) == 0) {
                            ShellExecuteW(NULL, L"open", available_disk_types[i].signin, NULL, NULL, SW_SHOWNORMAL);
                            break;
                        }
                    }
                    break;
                case IDOK:
                    disk = malloc(sizeof(NDISK));
                    memset(disk, 0, sizeof(NDISK));
                    GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, (LPWSTR)&buff, INPUT_MAX);
                    for(i = NDISK_FIRST_DISK; i <= NDISK_LAST_DISK; i++) {
                        if(wcscmp(buff, available_disk_types[i].description) == 0) {
                            disk->type_id = available_disk_types[i].type_id;
                            break;
                        }
                    }
                    GetDlgItemTextW(hWnd, IDC_EDIT_USERNAME, (LPWSTR)&buff, INPUT_MAX);
                    tmp = wtrim(buff);
                    if(wcscmp(tmp, L"") == 0) {
                        MessageBoxW(hWnd, L"没有输入用户名. ", L"提示", MB_OK);
                        break;
                    }
                    disk->username = _wcsdup(tmp);
                    GetDlgItemTextW(hWnd, IDC_EDIT_PASSWORD, (LPWSTR)&buff, INPUT_MAX);
                    tmp = wtrim(buff);
                    disk->password = _wcsdup(tmp);
                    GetDlgItemTextW(hWnd, IDC_EDIT_TITLE, (LPWSTR)&buff, INPUT_MAX);
                    tmp = wtrim(buff);
                    if(wcscmp(tmp, L"") == 0) {
                        memset(tmp, 0, INPUT_MAX);
                        wcscpy_s(tmp, INPUT_MAX, available_disk_types[i].description);
                        wcscat_s(tmp, INPUT_MAX, L"(");
                        wcscat_s(tmp, INPUT_MAX, disk->username);
                        wcscat_s(tmp, INPUT_MAX, L")");
                    }
                    disk->nickname = _wcsdup(tmp);
                    if(dict_exists(available_disks, disk->nickname)) {
                        if(MessageBoxW(hWnd, L"已经存在这个名称的网盘了，是否要覆盖已存在的网盘？", L"提示", MB_YESNO) == IDNO) {
                            ndisk_destroy(&disk);
                            return 1;
                        }
                    }
                    dict_set_element_s(available_disks, disk->nickname, disk, sizeof(NDISK), ndisk_destroy);
                    free(disk);
                    disk = NULL;
                    EndDialog(hWnd, IDOK);
                    return 0;
                    //L = luaL_newstate();
                    //luaL_openlibs(L);
                    //luaL_dofile(L, "test.lua");
                    //lua_close(L);
                    //http_connect(&conn, "login.live.com", 443U, TRUE);
                    //request = http_request_create("/", HTTP_METHOD_GET, NULL);
                    //http_request(conn, request, &response);
                    //http_request_destory(&request);
                    //request = NULL;
                    //http_response_destory(&response);
                    //response = NULL;
                    //http_disconnect(&conn);
                    //conn = NULL;
                    //http_connect(&conn, "news.163.com", 80U, FALSE);
                    //request = http_request_create("/", HTTP_METHOD_GET, NULL);
                    //http_request(conn, request, &response);
                    //http_request_destory(&request);
                    //request = NULL;
                    //http_response_destory(&response);
                    //response = NULL;
                    //http_disconnect(&conn);
                    //conn = NULL;
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return 1;
            }
            break;
    }
    return 0;
}