#ifndef NDPLUGIN_H_
#define NDPLUGIN_H_

#include <Windows.h>
#include <stdlib.h>

#define PATH_MAX 1024

#define FS_EXEC_OK 0
#define FS_EXEC_ERROR 1
#define FS_EXEC_YOURSELF -1
#define FS_EXEC_SYMLINK -2

#define MSGTYPE_CONNECT 1
#define MSGTYPE_DISCONNECT 2
#define MSGTYPE_DETAILS 3
#define MSGTYPE_TRANSFERCOMPLETE 4
#define MSGTYPE_CONNECTCOMPLETE 5
#define MSGTYPE_IMPORTANTERROR 6
#define MSGTYPE_OPERATIONCOMPLETE 7

typedef int (__stdcall *tProgressProc)(int, char*, char*,int);
typedef void (__stdcall *tLogProc)(int, int, char*);
typedef BOOL (__stdcall *tRequestProc)(int, int, char*, char*, char*, int);
typedef int (__stdcall *tProgressProcW)(int, WCHAR*, WCHAR*, int);
typedef void (__stdcall *tLogProcW)(int, int, WCHAR*);
typedef BOOL (__stdcall *tRequestProcW)(int, int, WCHAR*, WCHAR*, WCHAR*, int);

typedef struct _RemoteInfoStruct{
    DWORD SizeLow,SizeHigh;
    FILETIME LastWriteTime;
    int Attr;
} RemoteInfoStruct;

extern HANDLE hInst;
extern int PluginNumber;
extern tProgressProcW ProgressProcW;
extern tLogProcW LogProcW;
extern tRequestProcW RequestProcW;

#endif