#include <openssl/ssl.h>
#include "ui.h"
#include "resource.h"
#include "disk.h"
#include "http.h"
#include "ndplugin.h"
//#include "lua/lauxlib.h"

#define INPUT_MAX 1024

INT_PTR CALLBACK NewDiskDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
    WCHAR buff[INPUT_MAX];
    int i = 0;
	//HTTP_CONNECTION *conn;
 //   HTTP_REQUEST *request;
 //   HTTP_RESPONSE *response;
 //   lua_State *L;
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
                        }
                    }
                    break;
                case IDOK:
                    //L = luaL_newstate();
                    //luaL_openlibs(L);
                    //luaL_dofile(L, "test.lua");
                    //lua_close(L);
					//http_connect(&conn, "login.live.com", 443U, TRUE);
     //               request = http_request_create("/", HTTP_METHOD_GET, NULL);
					//http_request(conn, request, &response);
     //               http_request_destory(&request);
     //               request = NULL;
     //               http_response_destory(&response);
     //               response = NULL;
					//http_disconnect(&conn);
     //               conn = NULL;
     //               http_connect(&conn, "news.163.com", 80U, FALSE);
     //               request = http_request_create("/", HTTP_METHOD_GET, NULL);
     //               http_request(conn, request, &response);
     //               http_request_destory(&request);
     //               request = NULL;
     //               http_response_destory(&response);
     //               response = NULL;
     //               http_disconnect(&conn);
     //               conn = NULL;
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return 1;
            }
            break;
    }
    return 0;
}