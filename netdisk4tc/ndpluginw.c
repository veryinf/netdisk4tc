#include "ndplugin.h"
#include "resource.h"
#include "ui.h"
#include "utility.h"

HANDLE hInst;
int PluginNumber;
tProgressProcW ProgressProcW = NULL;
tLogProcW LogProcW = NULL;
tRequestProcW RequestProcW = NULL;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hInst = hModule;
    }
    return TRUE;
}

void __stdcall FsGetDefRootName(CHAR* rootName,int maxlen) {
    strlcpy(rootName, "Net Disks", maxlen);
}

int __stdcall FsInitW(int PluginNr, tProgressProcW pProgressProcW, tLogProcW pLogProcW, tRequestProcW pRequestProcW) {
    ProgressProcW = pProgressProcW;
    LogProcW = pLogProcW;
    RequestProcW = pRequestProcW;
    PluginNumber = PluginNr;
    return 0;
}

typedef struct {
    WCHAR PathW[PATH_MAX];
    WCHAR LastFoundNameW[PATH_MAX];
    HANDLE searchhandle;
} tLastFindStuct, *pLastFindStuct;

HANDLE __stdcall FsFindFirstW(WCHAR* Path, WIN32_FIND_DATAW *FindData)
{
    memset(FindData, 0, sizeof(WIN32_FIND_DATAW));
    if (wcscmp(Path, L"\\") == 0) {
        FindData->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
        FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
        FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
        wcslcpy(FindData->cFileName, L"[功能]新增网盘", _countof(FindData->cFileName) - 1);
        return NULL;
    }
    return INVALID_HANDLE_VALUE;
}

BOOL __stdcall FsFindNextW(HANDLE Hdl,WIN32_FIND_DATAW *FindData)
{
    WCHAR buf[PATH_MAX];
    pLastFindStuct lf;

    if ((int)Hdl==1)
        return FALSE;

    lf=(pLastFindStuct)Hdl;
    return FALSE;
}

int __stdcall FsGetFileW(WCHAR* RemoteName, WCHAR* LocalName, int CopyFlags, RemoteInfoStruct* ri) {
    return FS_EXEC_ERROR;
}

int __stdcall FsExecuteFileW(HWND MainWin, WCHAR* RemoteName, WCHAR* Verb) {
    if(wcscmp(RemoteName, L"\\[功能]新增网盘") == 0) {
        DialogBoxW((HINSTANCE)hInst, MAKEINTRESOURCEW(IDD_NEW), GetActiveWindow(), NewDiskDlgProc);
        return FS_EXEC_OK;
    }
    return FS_EXEC_ERROR;
}

int __stdcall FsFindClose(HANDLE Hdl)
{
    //pLastFindStuct lf;
    //if ((int)Hdl==1)
    //    return 0;
    //lf=(pLastFindStuct)Hdl;
    //if (lf->searchhandle!=INVALID_HANDLE_VALUE) {
    //    FindClose(lf->searchhandle);
    //    lf->searchhandle=INVALID_HANDLE_VALUE;
    //}
    //free(lf);
    //return 0;
}