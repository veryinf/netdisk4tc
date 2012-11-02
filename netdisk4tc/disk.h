#ifndef DISKS_H_
#define DISKS_H_
#include <stdlib.h>
#include "dictionary.h"

#define NDISK_FIRST_DISK    0x00
#define NDISK_SKYDRIVE      0x00
#define NDISK_KUAIPAN       0x01
#define NDISK_VDISK         0x02
#define NDISK_LAST_DISK     0x02

typedef struct _ndisk_type {
    unsigned short type_id;
    wchar_t * description;
    wchar_t * signin;
} NDISK_TYPE;

extern NDISK_TYPE available_disk_types[];
extern DICTIONARY *available_disks;

typedef struct _disk{
    unsigned short type_id;
    wchar_t *  username;
    wchar_t *  token;
    wchar_t *  secret;
    wchar_t * nickname;
} NDISK;



#endif