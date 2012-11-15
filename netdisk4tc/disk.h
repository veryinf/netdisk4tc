#ifndef DISKS_H_
#define DISKS_H_
#include <stdlib.h>
#include "dictionary.h"
#include "lua/lauxlib.h"

#define NDISK_OK 0
#define NDISK_FATAL 1

typedef struct _ndisk_entry {
    wchar_t * name;
    wchar_t * description;
    wchar_t * signin;
    lua_State *script;
} NDiskEntry;

extern NDiskEntry *available_disk_entries;
extern size_t available_disk_entries_length;
extern Dictionary *available_disks;

typedef struct _disk{
    wchar_t * type;
    wchar_t * username;
    wchar_t * password;
    wchar_t * nickname;
    wchar_t * token;
    wchar_t * secret;
} NDisk;

void ndisk_destroy(NDisk **);
void ndisks_save(void);
void ndisks_load(void);

//************************************
// Returns:   int，成功删除返回 NDISK_OK, 分析失败返回 NDISK_FATAL
// Description: 分析插件路径, 获得对应的 网盘 类型和 网盘
//************************************
int ndisk_parse(const wchar_t *, NDiskEntry **entry, NDisk **, wchar_t *);

//************************************
// Returns:   int，成功删除返回 NDISK_OK, 分析失败返回 NDISK_FATAL
// Description: 获取指定网盘指定目录的文件列表
//************************************
int ndisk_dir(Dictionary *, const NDiskEntry *, NDisk *, const wchar_t *);
#endif