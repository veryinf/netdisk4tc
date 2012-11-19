#ifndef UTILITY_H_
#define UTILITY_H_
#include <wchar.h>
#include <Windows.h>
#include "lua/lauxlib.h"

#define WCS_SIZEOF(str) ((wcslen(str) + 1) * sizeof(wchar_t))

wchar_t* wcslcpy(wchar_t*, const wchar_t*, int);
char* strlcpy(char*, const char*, int);
wchar_t* ctow(const char *);
char* wtoc(const wchar_t *);
char* trim(char *);
wchar_t* wtrim(wchar_t *);
int strtotime(wchar_t *, SYSTEMTIME *);
extern wchar_t my_dir[MAX_PATH];
extern lua_State *script;
void lua_reset(void);
char *convert(char *, int, int);
#endif
