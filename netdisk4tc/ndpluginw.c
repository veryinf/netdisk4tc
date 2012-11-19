#include <wchar.h>
#include "ndplugin.h"
#include "resource.h"
#include "ui.h"
#include "utility.h"
#include "disk.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "wrapper.h"

HANDLE hInst;
int PluginNumber;
tProgressProcW ProgressProcW = NULL;
tLogProcW LogProcW = NULL;
tRequestProcW RequestProcW = NULL;
wchar_t my_dir[MAX_PATH];
lua_State *script = NULL;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    int i = 0;
    WIN32_FIND_DATAW fData;
    wchar_t tmp[PATH_MAX];
    char *sTmp;
    HANDLE fHandle = NULL;
    lua_State *lua = NULL;
    NDiskEntry *pEntry = NULL;
    
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hInst = hModule;
        // init
        GetModuleFileNameW((HMODULE)hInst, my_dir, MAX_PATH);
        i = MAX_PATH;
        while (my_dir[--i] != '\\') {
            my_dir[i] = '\0';
        }
        wcslcpy(tmp, my_dir, PATH_MAX);
        wcscat_s(tmp, PATH_MAX, L"entries\\*.lua");
        fHandle = FindFirstFileW(tmp, &fData);
        i = 0;

        script = luaL_newstate();
        luaL_openlibs(script);
        wcslcpy(tmp, my_dir, PATH_MAX);
        wcscat_s(tmp, PATH_MAX, L"?.lua");
        sTmp = wtoc(tmp);
        lua_getglobal(script, "package");
        lua_pushstring(script, sTmp);
        lua_setfield(script, -2, "path");
        lua_pop(script, 1);
        free(sTmp);
        sTmp = NULL;
        lua_getglobal(script, "require");
        lua_pushstring(script, "Json");
        lua_call(script, 1, 1);
        lua_setglobal(script, "json");
        lua_register(script, "http", wp_http);
        lua_register(script, "expl", wp_explorer);
        //lua_register(script, "utoc", wp_utoc);
        lua_newtable(script);

        if(fHandle != INVALID_HANDLE_VALUE) {
            do {
                lua_reset();
                wcslcpy(tmp, my_dir, PATH_MAX);
                wcscat_s(tmp, PATH_MAX, L"entries\\");
                wcscat_s(tmp, PATH_MAX, fData.cFileName);
                sTmp = wtoc(tmp);
                if(luaL_loadfile(script, sTmp)) {
                    free(sTmp);
                    sTmp = NULL;
                    OutputDebugStringA("Error Msg is: ");
                    OutputDebugStringA(lua_tostring(script,-1));
                    continue;
                }
                free(sTmp);
                sTmp = NULL;
                _wsplitpath_s(fData.cFileName, NULL, 0, NULL, 0, (wchar_t *)&tmp, PATH_MAX, NULL, 0);
                sTmp = wtoc(tmp);
                lua_setfield(script, -2, sTmp);
                lua_getfield(script, -1, sTmp);
                free(sTmp);
                sTmp = NULL;
                if(lua_pcall(script, 0, 0, 0)) {
                    OutputDebugStringA("Error Msg is: ");
                    OutputDebugStringA(lua_tostring(script,-1));
                    continue;
                }
                lua_getglobal(script, "signin");
                if(!lua_isstring(script, -1)) {
                    lua_pop(script, 1);
                    continue;
                }
                lua_getglobal(script, "description");
                if(!lua_isstring(script, -1)) {
                    lua_pop(script, 1);
                    continue;
                }

                if(available_disk_entries == NULL) {
                    available_disk_entries = (NDiskEntry *)malloc(sizeof(NDiskEntry));
                } else {
                    pEntry = (NDiskEntry *)realloc(available_disk_entries, (i + 1) * sizeof(NDiskEntry));
                    if(pEntry != available_disk_entries) {
                        available_disk_entries = pEntry;
                    }
                }
                available_disk_entries[i].name = _wcsdup(tmp);
                available_disk_entries[i].signin = ctow(lua_tostring(script, -2));
                available_disk_entries[i].description = ctow(lua_tostring(script, -1));
                lua_pop(script, 2);

                i++;
            } while (FindNextFileW(fHandle, &fData));
        }
        lua_setglobal(script, "entries");
        available_disk_entries_length = i;
        available_disks = dict_initialize();
        ndisks_load();
        OleInitialize(0);
    }
    if(ul_reason_for_call == DLL_PROCESS_DETACH) {
        // destory
        lua_close(script);
        script = NULL;
        if(available_disk_entries != NULL) {
            for(i = 0; i < available_disk_entries_length; i++) {
                free(available_disk_entries[i].name);
                free(available_disk_entries[i].description);
                free(available_disk_entries[i].signin);
            }
        }
        if(available_disks != NULL) {
            dict_destroy(&available_disks);
        }
        OleUninitialize();
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
    NDisk * disk;
    NDiskEntry * entry;
    wchar_t sPath[PATH_MAX];
    wchar_t path[PATH_MAX];
    Dictionary * dict;
    DictEntry * findHandle;
    int isLast;
} FindStruc;

HANDLE __stdcall FsFindFirstW(WCHAR* Path, WIN32_FIND_DATAW *FindData) {
    FindStruc *pFind = NULL;
    pFind = (FindStruc *)malloc(sizeof(FindStruc));
    memset(pFind, 0, sizeof(FindStruc));
    if(wcscmp(Path, L"\\") == 0) {
        pFind->findHandle = available_disks->first;
    } else {
        if(ndisk_parse(Path, &pFind->entry, &pFind->disk, pFind->sPath) != NDISK_OK || pFind->disk == NULL) {
            free(pFind);
            return INVALID_HANDLE_VALUE;
        }
        pFind->dict = dict_initialize();
        if(ndisk_dir(pFind->dict, pFind->entry, pFind->disk, pFind->sPath) == NDISK_FATAL) {
            free(pFind);
            return INVALID_HANDLE_VALUE;
        }
        pFind->findHandle = pFind->dict->first;
    }
    wcslcpy(pFind->path, Path, PATH_MAX);
    memset(FindData, 0, sizeof(WIN32_FIND_DATAW));
    FindData->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
    FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
    FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
    wcslcpy(FindData->cFileName, L"..", _countof(FindData->cFileName) - 1);
    return pFind;
}

BOOL __stdcall FsFindNextW(HANDLE Hdl,WIN32_FIND_DATAW *FindData) {
    FindStruc *pFind = (FindStruc *)Hdl;
    WIN32_FIND_DATAW *fData = NULL;
    NDisk *disk = NULL;
    if(pFind == NULL || pFind->isLast) {
        return FALSE;
    }
    if (wcscmp(pFind->path, L"\\") == 0) {
        memset(FindData, 0, sizeof(WIN32_FIND_DATAW));
        if(pFind->findHandle == NULL) {
            FindData->dwFileAttributes = FILE_ATTRIBUTE_READONLY;
            FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
            FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
            wcslcpy(FindData->cFileName, L"[操作]新增网盘", _countof(FindData->cFileName) - 1);
            pFind->isLast = TRUE;
            return TRUE;
        } else {
            disk = (NDisk *)pFind->findHandle->value;
            FindData->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
            FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
            FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
            wcslcpy(FindData->cFileName, disk->nickname, _countof(FindData->cFileName) - 1);
            pFind->findHandle = pFind->findHandle->next;
            return TRUE;
        }
    } else {
        if(pFind->findHandle != NULL) {
            fData = (WIN32_FIND_DATAW *)pFind->findHandle->value;
            FindData->dwFileAttributes = fData->dwFileAttributes;
            FindData->nFileSizeHigh = fData->nFileSizeHigh;
            FindData->nFileSizeLow = fData->nFileSizeLow;
            FindData->ftCreationTime = fData->ftCreationTime;
            FindData->ftLastWriteTime = fData->ftLastWriteTime;
            FindData->ftLastAccessTime = fData->ftLastAccessTime;
            wcslcpy(FindData->cFileName, fData->cFileName, _countof(FindData->cFileName) - 1);
            pFind->findHandle = pFind->findHandle->next;
            return TRUE;
        }
    }
    return FALSE;
}

int __stdcall FsFindClose(HANDLE Hdl) {
    FindStruc *pFind = (FindStruc *)Hdl;
    if(pFind != NULL) {
        if(pFind->dict != NULL) {
            dict_destroy(&pFind->dict);
        }
        free(pFind);
        pFind = NULL;
    }
    return 0;
}

int __stdcall FsGetFileW(WCHAR* RemoteName, WCHAR* LocalName, int CopyFlags, RemoteInfoStruct* ri) {
    return FS_EXEC_ERROR;
}

int __stdcall FsExecuteFileW(HWND MainWin, WCHAR* RemoteName, WCHAR* Verb) {
    INT_PTR dlgResult = 0;
    if(wcscmp(Verb, L"open") == 0 && wcscmp(RemoteName, L"\\[操作]新增网盘") == 0) {
        if(!available_disk_entries_length) {
            RequestProcW(PluginNumber, RT_MsgOK, L"配置错误", L"网盘脚本错误, 没有设置任何有效的网盘配置文件. ", NULL, 0);
            return FS_EXEC_ERROR;
        }
        dlgResult = DialogBoxW((HINSTANCE)hInst, MAKEINTRESOURCEW(IDD_NEW), GetActiveWindow(), NewdiskProc);
        if(dlgResult == IDOK) {
            wcslcpy(RemoteName, L"\\", PATH_MAX);
            return FS_EXEC_SYMLINK;
        }
        return FS_EXEC_OK;
    }
    return FS_EXEC_ERROR;
}
