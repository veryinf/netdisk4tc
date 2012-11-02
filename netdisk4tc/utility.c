#include <string.h>
#include <Windows.h>
#include "utility.h"

wchar_t* wcslcpy(wchar_t *str1, const wchar_t *str2, int imaxlen) {
    if ((int)wcslen(str2) >= imaxlen - 1) {
        wcsncpy(str1, str2, imaxlen - 1);
        str1[imaxlen - 1] = 0;
    } else {
        wcscpy(str1, str2);
    }
    return str1;
}

char* strlcpy(char *str1, const char *str2, int imaxlen) {
    if ((int)strlen(str2) >= imaxlen - 1) {
        strncpy(str1, str2, imaxlen - 1);
        str1[imaxlen - 1] = 0;
    } else {
        strcpy(str1, str2);
    }
    return str1;
}

wchar_t* ctow(const char *str) {
    wchar_t* buffer = NULL;
    size_t size = 0;
    if(str) {
        size =(size_t) MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
        buffer = (wchar_t *)calloc(size, sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, size); 
    }
    return buffer;
}