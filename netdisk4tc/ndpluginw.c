#include "ndplugin.h"
#include "resource.h"
#include "ui.h"
#include "utility.h"
#include "disk.h"
#include "lua/lauxlib.h"

HANDLE hInst;
int PluginNumber;
tProgressProcW ProgressProcW = NULL;
tLogProcW LogProcW = NULL;
tRequestProcW RequestProcW = NULL;
wchar_t my_dir[MAX_PATH];

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    int i = 0;
    WIN32_FIND_DATAW fData;
    wchar_t tmp[PATH_MAX];
    char *sTmp;
    HANDLE fHandle = NULL;
    lua_State *lua = NULL;
    NDisk_Entry *pEntry = NULL;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hInst = hModule;
        // init
        GetModuleFileNameW(hInst, my_dir, MAX_PATH);
        i = MAX_PATH;
        while (my_dir[--i] != '\\') {
            my_dir[i] = '\0';
        }
        wcslcpy(tmp, my_dir, PATH_MAX);
        wcscat_s(tmp, PATH_MAX, L"entries\\*.lua");
        fHandle = FindFirstFileW(tmp, &fData);
        if(fHandle) {
            i = 0;
            while(TRUE) {
                lua = NULL;
                lua = luaL_newstate();
                luaL_openlibs(lua);
                wcslcpy(tmp, my_dir, PATH_MAX);
                wcscat_s(tmp, PATH_MAX, L"entries\\");
                wcscat_s(tmp, PATH_MAX, fData.cFileName);
                sTmp = wtoc(tmp);
                if(luaL_loadfile(lua, sTmp) || lua_pcall(lua, 0, 0, 0)) {
                    lua_close(lua);
                    if(!FindNextFileW(fHandle, &fData)) {
                        break;
                    }
                    continue;
                }
                free(sTmp);
                sTmp = NULL;
                lua_getglobal(lua, "signin");
                lua_getglobal(lua, "description");
                if(!lua_isstring(lua, -2)) {
                    lua_close(lua);
                    if(!FindNextFileW(fHandle, &fData)) {
                        break;
                    }
                    continue;
                }
                if(!lua_isstring(lua, -1)) {
                    lua_close(lua);
                    if(!FindNextFileW(fHandle, &fData)) {
                        break;
                    }
                    continue;
                }
                if(available_disk_entries == NULL) {
                    available_disk_entries = malloc(sizeof(NDisk_Entry));
                } else {
                    pEntry = realloc(available_disk_entries, (i + 1) * sizeof(NDisk_Entry));
                    if(pEntry != available_disk_entries) {
                        available_disk_entries = pEntry;
                    }
                }
                available_disk_entries[i].type_id = i;
                available_disk_entries[i].description = ctow(lua_tostring(lua, -1));
                available_disk_entries[i].signin = ctow(lua_tostring(lua, -2));
                available_disk_entries[i].script = lua;
                i++;
                if(!FindNextFileW(fHandle, &fData)) {
                    break;
                }
            }
        }
        available_disk_entries_length = i;
        available_disks = dict_initialize();
    }
    if(ul_reason_for_call == DLL_PROCESS_DETACH) {
        // destory
        if(available_disk_entries != NULL) {
            for(i = 0; i < available_disk_entries_length; i++) {
                free(available_disk_entries[i].description);
                free(available_disk_entries[i].signin);
                lua_close(available_disk_entries[i].script);
            }
        }
        if(available_disks != NULL) {
            dict_destroy(&available_disks);
        }
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
    wchar_t subPath[PATH_MAX];
    wchar_t path[PATH_MAX];
    DictEntry * findHandle;
    int isLast;
} FindStruc;

HANDLE __stdcall FsFindFirstW(WCHAR* Path, WIN32_FIND_DATAW *FindData) {
    FindStruc *pFind = NULL;
    wchar_t * tmp = NULL, *sTmp = NULL, key[PATH_MAX];
    pFind = malloc(sizeof(FindStruc));
    memset(pFind, 0, sizeof(FindStruc));
    if(wcscmp(Path, L"\\") == 0) {
        pFind->findHandle = available_disks->first;
    } else {
        tmp = Path + 1;
        for(sTmp = tmp; *sTmp != '\0' && *sTmp != '\\'; ++sTmp){
            //pass
        }
        memset(key, '\0', PATH_MAX);
        wcslcpy(key, tmp, sTmp - tmp + 1);
        memset(pFind->subPath, '\0', PATH_MAX);
        wcslcpy(pFind->subPath, sTmp, PATH_MAX);
        pFind->disk = dict_get_element(available_disks, key);
        if(pFind->disk == NULL) {
            free(pFind);
            return INVALID_HANDLE_VALUE;
        }
        if(!pFind->disk->token) {
            LogProcW(PluginNumber, MSGTYPE_DETAILS, key);
        }
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
    NDisk *disk = NULL;
    if(pFind == NULL || pFind->isLast) {
        return FALSE;
    }
    if (wcscmp(pFind->path, L"\\") == 0) {
        memset(FindData, 0, sizeof(WIN32_FIND_DATAW));
        if(pFind->findHandle == NULL) {
            FindData->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
            FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
            FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
            wcslcpy(FindData->cFileName, L"[操作]新增网盘", _countof(FindData->cFileName) - 1);
            pFind->isLast = TRUE;
            return pFind;
        } else {
            disk = (NDisk *)pFind->findHandle->value;
            FindData->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
            FindData->ftLastWriteTime.dwHighDateTime = 0xFFFFFFFF;
            FindData->ftLastWriteTime.dwLowDateTime = 0xFFFFFFFE;
            wcslcpy(FindData->cFileName, disk->nickname, _countof(FindData->cFileName) - 1);
            pFind->findHandle = pFind->findHandle->next;
            return pFind;
        }
    }
    return FALSE;
}

int __stdcall FsFindClose(HANDLE Hdl) {
    FindStruc *pFind = (FindStruc *)Hdl;
    if(pFind != NULL) {
        free(pFind);
        pFind = NULL;
    }
    return 0;
}

int __stdcall FsGetFileW(WCHAR* RemoteName, WCHAR* LocalName, int CopyFlags, RemoteInfoStruct* ri) {
    return FS_EXEC_ERROR;
}

int __stdcall FsExecuteFileW(HWND MainWin, WCHAR* RemoteName, WCHAR* Verb) {
    int dlgResult = 0;
    if(wcscmp(RemoteName, L"\\[操作]新增网盘") == 0) {
        dlgResult = DialogBoxW((HINSTANCE)hInst, MAKEINTRESOURCEW(IDD_NEW), GetActiveWindow(), NewDiskDlgProc);
        if(dlgResult == IDOK) {
            wcslcpy(RemoteName, L"\\", PATH_MAX);
            return FS_EXEC_SYMLINK;
        }
        return FS_EXEC_OK;
    }
    return FS_EXEC_ERROR;
}
