#ifndef DISKS_H_
#define DISKS_H_

#include <Windows.h>

typedef struct _disk DISK;
struct _disk{
    WCHAR username[100];
    WCHAR password[100];
    WCHAR token[100];
    WCHAR secret[100];
    DISK *pNext;
};

#endif