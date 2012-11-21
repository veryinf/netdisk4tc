#include "ui.h"
#include "resource.h"
#include "disk.h"
#include "ndplugin.h"
#include "webbrowser\cwebpage.h"

#define INPUT_MAX 1024

INT_PTR CALLBACK NewdiskProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
    WCHAR buff[INPUT_MAX];
    WCHAR *tmp;
    NDisk *disk = NULL;
    int i = 0;
    switch (Message) {
        case WM_INITDIALOG:
            for(i = 0; i < available_disk_entries_length; i++) {
                SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)available_disk_entries[i].description);
            }
            SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_SETCURSEL, 0, (LPARAM)NULL);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDC_BUTTON_REGISTER:
                    GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, (LPWSTR)&buff, INPUT_MAX);
                    for(i = 0; i < available_disk_entries_length; i++) {
                        if(wcscmp(buff, available_disk_entries[i].description) == 0) {
                            ShellExecuteW(NULL, L"open", available_disk_entries[i].signin, NULL, NULL, SW_SHOWNORMAL);
                            break;
                        }
                    }
                    break;
                case IDOK:
                    disk = (NDisk *)malloc(sizeof(NDisk));
                    memset(disk, 0, sizeof(NDisk));
                    GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, (LPWSTR)&buff, INPUT_MAX);
                    for(i = 0; i < available_disk_entries_length; i++) {
                        if(wcscmp(buff, available_disk_entries[i].description) == 0) {
                            disk->type = _wcsdup(available_disk_entries[i].name);
                            break;
                        }
                    }
                    GetDlgItemTextW(hWnd, IDC_EDIT_USERNAME, (LPWSTR)&buff, INPUT_MAX);
                    tmp = wtrim(buff);
                    if(wcscmp(tmp, L"") == 0) {
                        MessageBoxW(hWnd, L"没有输入用户名.", L"提示", MB_OK);
                        break;
                    }
                    disk->username = _wcsdup(tmp);
                    GetDlgItemTextW(hWnd, IDC_EDIT_TITLE, (LPWSTR)&buff, INPUT_MAX);
                    tmp = wtrim(buff);
                    if(wcscmp(tmp, L"") == 0) {
                        memset(tmp, 0, INPUT_MAX);
                        wcscpy_s(tmp, INPUT_MAX, available_disk_entries[i].description);
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
                    dict_set_element_s(available_disks, disk->nickname, disk, sizeof(NDisk), ndisk_destroy_s);
                    ndisks_save();
                    free(disk);
                    disk = NULL;
                    EndDialog(hWnd, IDOK);
                    return 0;
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    return 1;
            }
            break;
    }
    return 0;
}

INT_PTR CALLBACK AuthProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
    wchar_t *data = NULL;
    switch(Message) {
        case WM_SIZE:
            ResizeBrowser(hWnd, LOWORD(lParam), HIWORD(lParam));
            return(0);
        case WM_CREATE:
            if (EmbedBrowserObject(hWnd)){ 
                return(-1);
            }
            return(0);
        case WM_APP:
            PostMessage(hWnd, WM_USER_AUTHTOKENDATA, wParam, 0);
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case WM_DESTROY:
            UnEmbedBrowserObject(hWnd);
            PostQuitMessage(0);
            return(TRUE);
    }
    return(DefWindowProc(hWnd, Message, wParam, lParam));
}
