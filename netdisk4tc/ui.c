#include "ui.h"
#include "resource.h"
#include "disk.h"
#include "dictionary.h"
#include "ndplugin.h"

#define INPUT_MAX 1024

int __stdcall NewDiskDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam) {
	WCHAR buff[INPUT_MAX];
	DICTIONARY *dict;
	WCHAR *tmp;
	WCHAR *kuaipan;
	int ss;
	switch (Message) {
		case WM_INITDIALOG:
			SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)L"金山快盘");
			SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_ADDSTRING, 0, (LPARAM)L"微盘");
			SendDlgItemMessageW(hWnd, IDC_COMBOBOX_TYPE, CB_SETCURSEL, 0, NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BUTTON_REGISTER:
					GetDlgItemTextW(hWnd, IDC_COMBOBOX_TYPE, &buff, INPUT_MAX);
					if(wcscmp(buff, L"金山快盘") == 0) {
						ShellExecuteW(NULL, L"open", L"http://www.kuaipan.cn/account_register.htm", NULL, NULL, SW_SHOWNORMAL);
					}
					if(wcscmp(buff, L"微盘") == 0) {
						ShellExecuteW(NULL, L"open", L"http://weibo.com/signup/signup.php", NULL, NULL, SW_SHOWNORMAL);
					}
					break;
				case IDOK:
					//dict = InitializeDictionary();
					//tmp = L"http://www.kuaipan.cn/account_register.htm";
					//DictionarySetElement(dict, L"kuaipan", (void *)tmp, (wcslen(tmp) + 1) * sizeof(WCHAR));
					//tmp = L"http://weibo.com/signup/signup.php";
					//DictionarySetElement(dict, L"vdisk", (void *)tmp, (wcslen(tmp) + 1) * sizeof(WCHAR));
					//kuaipan = (WCHAR *)DictionaryGetElement(dict, L"vdisk");
					//DictionaryRemoveElement(dict, L"vdisk");
					//FreeDictionary(dict);
					//dict = NULL;
					break;
				case IDCANCEL:
					EndDialog(hWnd, IDCANCEL);
					return 1;
			}
			break;
	}
	return 0;
}