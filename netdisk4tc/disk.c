#include <stdio.h>
#include "ndplugin.h"
#include "utility.h"
#include "disk.h"
#include "lua/lauxlib.h"

static wchar_t *_config_file = L"netdisks.ini";
NDiskEntry *available_disk_entries = NULL;
size_t available_disk_entries_length = 0;
Dictionary *available_disks = NULL;

static int ndisk_save_enumerator(const wchar_t *key, const void *value, size_t size, void **data) {
    NDisk *disk;
    disk = (NDisk *)value;
    WritePrivateProfileStringW(key, L"type", disk->type, (wchar_t*)data);
    WritePrivateProfileStringW(key, L"username", disk->username, (wchar_t*)data);
    WritePrivateProfileStringW(key, L"password", disk->password, (wchar_t*)data);
    WritePrivateProfileStringW(key, L"nickname", disk->nickname, (wchar_t*)data);
    WritePrivateProfileStringW(key, L"token", disk->token, (wchar_t*)data);
    WritePrivateProfileStringW(key, L"secret", disk->secret, (wchar_t*)data);
    return 1;
}

void ndisks_save() {
    wchar_t tmp[PATH_MAX];
    if(available_disks == NULL || !available_disks->length) {
        return;
    }
    wcslcpy(tmp, my_dir, PATH_MAX);
    wcscat_s(tmp, PATH_MAX, _config_file);
    DeleteFileW(tmp);
    dict_traverse(available_disks, ndisk_save_enumerator, (void **)&tmp);
}

#define SECTIONS_BUFFER_SIZE 4096
void ndisks_load() {
    wchar_t tmp[PATH_MAX], sTmp[SECTIONS_BUFFER_SIZE], *pTmp, rTmp[PATH_MAX];
    size_t i = 0, j = 0;
    NDisk disk;
    if(available_disks == NULL) {
        return;
    }
    wcslcpy(tmp, my_dir, PATH_MAX);
    wcscat_s(tmp, PATH_MAX, _config_file);
    GetPrivateProfileSectionNamesW(sTmp, SECTIONS_BUFFER_SIZE, tmp);
    for(i = 0; i < SECTIONS_BUFFER_SIZE; i++) {
        if(sTmp[0] == '\0') {
            break;
        }
        if(sTmp[i] == '\0') {
            pTmp = sTmp + j;
            if(!pTmp || wcscmp(pTmp, L"") == 0) {
                continue;
            }
            j = i + 1;
            memset(&disk, 0, sizeof(NDisk));
            GetPrivateProfileStringW(pTmp, L"type", NULL, rTmp, PATH_MAX, tmp);
            if(!rTmp || wcscmp(rTmp, L"") == 0) {
                continue;
            }
            disk.type = _wcsdup(rTmp);
            GetPrivateProfileStringW(pTmp, L"username", NULL, rTmp, PATH_MAX, tmp);
            if(!rTmp || wcscmp(rTmp, L"") == 0) {
                free(disk.type);
                continue;
            }
            disk.username = _wcsdup(rTmp);
            disk.nickname = _wcsdup(pTmp);
            GetPrivateProfileStringW(pTmp, L"password", NULL, rTmp, PATH_MAX, tmp);
            disk.password = _wcsdup(rTmp);
            GetPrivateProfileStringW(pTmp, L"token", NULL, rTmp, PATH_MAX, tmp);
            disk.token = _wcsdup(rTmp);
            GetPrivateProfileStringW(pTmp, L"secret", NULL, rTmp, PATH_MAX, tmp);
            disk.secret = _wcsdup(rTmp);
            dict_set_element_s(available_disks, pTmp, &disk, sizeof(NDisk), ndisk_destroy_s);
        }
    }
}

// hiden danger for sPath buffer size;
int ndisk_parse(const wchar_t *path, NDiskEntry **entry, NDisk **disk, wchar_t *sPath) {
    wchar_t tmp[PATH_MAX], *sTmp = NULL;
    int r = NDISK_FATAL, i = 0;
    if(!path || wcscmp(path, L"") == 0 || wcscmp(path, L"\\") == 0) {
        return r;
    }
    sTmp = (wchar_t *)path + 1;
    while(*sTmp != '\0' && *sTmp != '\\') {
        sTmp++;
    }
    wcslcpy(tmp, path + 1, (int)(sTmp - path));
    if(dict_get_element_s(available_disks, tmp, (void **)disk) == DICT_OK) {
        for(i = 0; i < available_disk_entries_length; i++) {
            if(wcscmp((*disk)->type, available_disk_entries[i].name) == 0) {
                *entry = &available_disk_entries[i];
                wcslcpy(sPath, path + wcslen(tmp) + 1, PATH_MAX);
                if(*sPath == '\0') {
                    wcslcpy(sPath, L"\\", PATH_MAX);
                }
                r = NDISK_OK;
                break;
            }
        }
    }
    return r;
}

static int ndisk_lua_push(lua_State *l, NDisk *disk) {
    char *tmp = NULL;
    wchar_t sTmp[40];
    lua_newtable(l);
    tmp = wtoc(disk->username);
    lua_pushstring(l, tmp);
    free(tmp);
    tmp = NULL;
    lua_setfield(l, -2, "username");
    if(disk->password == NULL || wcscmp(disk->password, L"") == 0) {
        memset(sTmp, 0, 40);
        if(RequestProcW(PluginNumber, RT_Password, L"请求密码", L"未保存密码, 或密码已失效, 请重新提供密码 . ", sTmp, 40) == FALSE) {
            return NDISK_FATAL;
        }
        disk->password = _wcsdup(sTmp);
    }
    tmp = wtoc(disk->password);
    lua_pushstring(l, tmp);
    free(tmp);
    tmp = NULL;
    lua_setfield(l, -2, "password");
    if(disk->token) {
        tmp = wtoc(disk->token);
        lua_pushstring(l, tmp);
        free(tmp);
        tmp = NULL;
        lua_setfield(l, -2, "token");
    }
    if(disk->secret) {
        tmp = wtoc(disk->secret);
        lua_pushstring(l, tmp);
        free(tmp);
        tmp = NULL;
        lua_setfield(l, -2, "secret");
    }
    return NDISK_OK;
}

int ndisk_dir(Dictionary *dict, const NDiskEntry *entry, NDisk *disk, const wchar_t *path) {
    WIN32_FIND_DATAW fData;
    char *tmp = NULL, *sTmp = NULL;
    wchar_t *pTmp = NULL;
    unsigned long size = 0;
    int i = 0;
    SYSTEMTIME time;
    lua_getglobal(entry->script, "dir");
    if(!lua_isfunction(entry->script, -1)) {
        RequestProcW(PluginNumber, RT_MsgOK, L"配置错误", L"网盘脚本错误, 不能读取当前目录的内容. ", NULL, 0);
        return NDISK_FATAL;
    }
    if(ndisk_lua_push(entry->script, disk) == NDISK_FATAL) {
        lua_settop(entry->script, 0);
        return NDISK_FATAL;
    }
    tmp = wtoc(path);
    lua_pushstring(entry->script, tmp);
    free(tmp);
    tmp = NULL;
    lua_setfield(entry->script, -2, "path");
    if(lua_pcall(entry->script, 1, 1, 0)) {
        OutputDebugStringA("Error Msg is: ");
        OutputDebugStringA(lua_tostring(entry->script, -1));
    }

    if(lua_istable(entry->script, -1)) {
        lua_pushnil(entry->script);
        while (lua_next(entry->script, -2)) {
            if(lua_istable(entry->script, -1)) {
                memset(&fData, 0, sizeof(WIN32_FIND_DATAW));
                lua_pushnil(entry->script);
                while(lua_next(entry->script, -2)) {
                    tmp = (char *)lua_tostring(entry->script, -2);
                    if(strcmp(tmp, "attribute") == 0) {
                        sTmp = (char *)lua_tostring(entry->script, -1);
                        if(strcmp(sTmp, "file") == 0) {
                            fData.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
                        }
                        if(strcmp(sTmp, "directory") == 0) {
                            fData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
                        }
                    }
                    if(strcmp(tmp, "filename") == 0) {
                        sTmp = (char *)lua_tostring(entry->script, -1);
                        pTmp = ctow(sTmp);
                        wcslcpy(fData.cFileName, pTmp, PATH_MAX);
                        free(pTmp);
                        pTmp = NULL;
                    }
                    if(strcmp(tmp, "size") == 0) {
                        size = (unsigned long)lua_tonumber(entry->script, -1);
                        fData.nFileSizeHigh = HIWORD(size);
                        fData.nFileSizeLow = LOWORD(size);
                    }
                    if(strcmp(tmp, "create") == 0) {
                        sTmp = (char *)lua_tostring(entry->script, -1);
                        pTmp = ctow(sTmp);
                        memset(&time, 0, sizeof(SYSTEMTIME));
                        if(strtotime(pTmp, &time)) {
                            SystemTimeToFileTime(&time, &fData.ftCreationTime);
                        }
                        free(pTmp);
                        pTmp = NULL;
                    }
                    if(strcmp(tmp, "access") == 0) {
                        sTmp = (char *)lua_tostring(entry->script, -1);
                        pTmp = ctow(sTmp);
                        memset(&time, 0, sizeof(SYSTEMTIME));
                        if(strtotime(pTmp, &time)) {
                            SystemTimeToFileTime(&time, &fData.ftLastAccessTime);
                        }
                        free(pTmp);
                        pTmp = NULL;
                    }
                    if(strcmp(tmp, "write") == 0) {
                        sTmp = (char *)lua_tostring(entry->script, -1);
                        pTmp = ctow(sTmp);
                        memset(&time, 0, sizeof(SYSTEMTIME));
                        if(strtotime(pTmp, &time)) {
                            SystemTimeToFileTime(&time, &fData.ftLastWriteTime);
                        }
                        free(pTmp);
                        pTmp = NULL;
                    }
                    lua_pop(entry->script, 1);
                }
                if(fData.cFileName) {
                    dict_set_element(dict, fData.cFileName, &fData, sizeof(WIN32_FIND_DATAW));
                }
            }
            lua_pop(entry->script, 1);
        }
    }
    lua_pop(entry->script, 1);
    return NDISK_OK;
}

int ndisk_download(const NDiskEntry *entry, NDisk *disk, const wchar_t *remote, const wchar_t *local) {
    return NDISK_OK;
}

int ndisk_upload(const NDiskEntry *entry, NDisk *disk, const wchar_t *local, const wchar_t *remote) {
    return NDISK_OK;
}

int ndisk_copy(const NDiskEntry *entry, NDisk *disk, const wchar_t *src, const wchar_t *des) {

    return NDISK_OK;
}

int ndisk_move(const NDiskEntry *entry, NDisk *disk, const wchar_t *src, const wchar_t *des) {

    return NDISK_OK;
}

int ndisk_erasure(const NDiskEntry *entry, NDisk *disk, const wchar_t *path) {

    return NDISK_OK;
}

int ndisk_mkdir(const NDiskEntry *entry, NDisk *disk, const wchar_t *path) {
    return NDISK_OK;
}

int ndisk_info(const NDiskEntry *entry, NDisk *disk, const wchar_t *path) {
    return NDISK_OK;
}

void ndisk_destroy_s(void **disk) {
    ndisk_destroy((NDisk **)disk);
}

void ndisk_destroy(NDisk **disk) {
    if((*disk) == NULL) {
        return;
    }
    if((*disk)->type != NULL) {
        free((*disk)->type);
        (*disk)->type = NULL;
    }
    if((*disk)->nickname != NULL) {
        free((*disk)->nickname);
        (*disk)->nickname = NULL;
    }
    if((*disk)->password != NULL) {
        free((*disk)->password);
        (*disk)->password = NULL;
    }
    if((*disk)->secret != NULL) {
        free((*disk)->secret);
        (*disk)->secret = NULL;
    }
    if((*disk)->token != NULL) {
        free((*disk)->token);
        (*disk)->token = NULL;
    }
    if((*disk)->username != NULL) {
        free((*disk)->username);
        (*disk)->username = NULL;
    }
    (*disk) = NULL;
}
