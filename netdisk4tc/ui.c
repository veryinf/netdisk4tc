#include "ui.h"
#include "resource.h"
#include "disk.h"
#include "http.h"
#include "ndplugin.h"

#define INPUT_MAX 1024

INT_PTR CALLBACK NewDiskDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
    WCHAR buff[INPUT_MAX];
	HTTP_CONNECTION *conn;
    HTTP_REQUEST *request;
    HTTP_RESPONSE *response;
    switch (Message) {
        case WM_INITDIALOG:
            SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)L"金山快盘");
            SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)L"微盘");
            SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_SETCURSEL, 0, (LPARAM)NULL);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_BUTTON_REGISTER:
                    GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, (LPWSTR)&buff, INPUT_MAX);
                    if(wcscmp(buff, L"金山快盘") == 0) {
                        ShellExecuteW(NULL, L"open", L"http://www.kuaipan.cn/account_register.htm", NULL, NULL, SW_SHOWNORMAL);
                    }
                    if(wcscmp(buff, L"微盘") == 0) {
                        ShellExecuteW(NULL, L"open", L"http://weibo.com/signup/signup.php", NULL, NULL, SW_SHOWNORMAL);
                    }
                    break;
                case IDOK:
					http_connect(&conn, "localhost", 80U);
                    request = http_request_create("/", HTTP_METHOD_GET, NULL);
					http_request(conn, request, &response);
                    http_request_destory(&request);
					http_disconnect(&conn);
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return 1;
            }
            break;
    }
    return 0;
}