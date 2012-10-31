#include "ndplugin.h"

//ignore
int __stdcall FsInit(int PluginNr, tProgressProc pProgressProc, tLogProc pLogProc, tRequestProc pRequestProc) {
    return 0;
}

//ignore
HANDLE __stdcall FsFindFirst(char* Path,WIN32_FIND_DATA *FindData) {
    return NULL;
}

//ignore
BOOL __stdcall FsFindNext(HANDLE Hdl,WIN32_FIND_DATA *FindData)
{
    return FALSE;
}

//ignore
int __stdcall FsGetFile(char* RemoteName, char* LocalName, int CopyFlags, RemoteInfoStruct* ri) {
    return 0;
}

//ignore
int __stdcall FsExecuteFile(HWND MainWin, char* RemoteName, char* Verb) {
    return FS_EXEC_ERROR;
}