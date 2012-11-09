#ifndef UTILITY_H_
#define UTILITY_H_
#include <wchar.h>
#include <Windows.h>

#define WCS_SIZEOF(str) ((wcslen(str) + 1) * sizeof(wchar_t))

wchar_t* wcslcpy(wchar_t*, const wchar_t*, int);
char* strlcpy(char*, const char*, int);
wchar_t* ctow(const char *str);
char* trim(char *);
wchar_t* wtrim(wchar_t *);
extern wchar_t my_dir[MAX_PATH];
#endif