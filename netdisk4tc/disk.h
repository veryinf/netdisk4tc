#ifndef DISKS_H_
#define DISKS_H_
#include <stdlib.h>
#include "dictionary.h"
#include "lua/lauxlib.h"

typedef struct _ndisk_type {
    unsigned short type_id;
    wchar_t * description;
    wchar_t * signin;
    lua_State *script;
} NDisk_Entry;

extern NDisk_Entry *available_disk_entries;
extern size_t available_disk_entries_length;
extern Dictionary *available_disks;

typedef struct _disk{
    unsigned short type_id;
    wchar_t * username;
    wchar_t * password;
    wchar_t * nickname;
    wchar_t * token;
    wchar_t * secret;
} NDisk;

void ndisk_destroy(NDisk **);

#endif