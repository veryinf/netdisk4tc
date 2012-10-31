#ifndef UTILITY_H_
#define UTILITY_H_
#include <wchar.h>

#define WCS_SIZEOF(str) ((wcslen(str) + 1) * sizeof(wchar_t))

wchar_t* wcslcpy(wchar_t*, const wchar_t*, int);
char* strlcpy(char*, const char*, int);

#endif