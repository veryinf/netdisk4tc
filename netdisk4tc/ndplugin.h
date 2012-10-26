#ifndef NDPLUGIN_H_
#define NDPLUGIN_H_

#include <Windows.h>
#include <stdlib.h>

#define PATH_MAX 1024

#define FS_EXEC_OK 0
#define FS_EXEC_ERROR 1
#define FS_EXEC_YOURSELF -1
#define FS_EXEC_SYMLINK -2

typedef int (__stdcall *tProgressProc)(int PluginNr,char* SourceName, char* TargetName,int PercentDone);
typedef void (__stdcall *tLogProc)(int PluginNr,int MsgType,char* LogString);
typedef BOOL (__stdcall *tRequestProc)(int PluginNr,int RequestType,char* CustomTitle,char* CustomText,char* ReturnedText,int maxlen);
typedef int (__stdcall *tProgressProcW)(int PluginNr,WCHAR* SourceName, WCHAR* TargetName,int PercentDone);
typedef void (__stdcall *tLogProcW)(int PluginNr,int MsgType,WCHAR* LogString);
typedef BOOL (__stdcall *tRequestProcW)(int PluginNr,int RequestType,WCHAR* CustomTitle, WCHAR* CustomText,WCHAR* ReturnedText,int maxlen);

typedef struct _RemoteInfoStruct{
	DWORD SizeLow,SizeHigh;
	FILETIME LastWriteTime;
	int Attr;
} RemoteInfoStruct;

#endif