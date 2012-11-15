#include <string.h>
#include <Windows.h>
#include <tchar.h>
#include "utility.h"

wchar_t* wcslcpy(wchar_t *str1, const wchar_t *str2, int imaxlen) {
    if ((int)wcslen(str2) >= imaxlen - 1) {
        wcsncpy_s(str1, imaxlen, str2, imaxlen - 1);
        str1[imaxlen - 1] = 0;
    } else {
        wcscpy_s(str1, imaxlen, str2);
    }
    return str1;
}

char* strlcpy(char *str1, const char *str2, int imaxlen) {
    if ((int)strlen(str2) >= imaxlen - 1) {
        strncpy_s(str1, imaxlen, str2, imaxlen - 1);
        str1[imaxlen - 1] = 0;
    } else {
        strcpy_s(str1, imaxlen, str2);
    }
    return str1;
}

wchar_t* ctow(const char *str) {
    wchar_t* buffer = NULL;
    size_t size = 0;
    if(str) {
        size =(size_t) MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
        buffer = (wchar_t *)calloc(size, sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, (int)size); 
    }
    return buffer;
}

char* wtoc(const wchar_t *str) {
    char* buffer = NULL;
    size_t size = 0;
    if(str) {
        size =(size_t) WideCharToMultiByte(CP_OEMCP, 0, str, -1, NULL, 0, NULL, FALSE);
        buffer = (char *)calloc(size, sizeof(char));
        WideCharToMultiByte(CP_OEMCP, 0, str, -1, buffer, (int)size, NULL, FALSE);
    }
    return buffer;
}

char* trim(char *str) {
    char *str_last, *str_cur;
    if(str == NULL) {
        return NULL;
    }
    for(; *str == ' ' || *str == '\r' || *str == '\n' || *str == '\t'; ++str) {

    }
    for(str_last = str_cur = str; *str_cur != '\0'; ++str_cur){
        if(*str_cur != ' ' && *str_cur != '\r' && *str_cur != '\n' && *str_cur != '\t') {
            str_last = str_cur;
        }
    }
    *++str_last = '\0';
    return str;
}

wchar_t* wtrim(wchar_t *wstr) {
    wchar_t *wstr_last, *wstr_cur;
    if(wstr == NULL) {
        return NULL;
    }
    for(; *wstr == ' ' || *wstr == '\r' || *wstr == '\n' || *wstr == '\t'; ++wstr) {

    }
    for(wstr_last = wstr_cur = wstr; *wstr_cur != '\0'; ++wstr_cur){
        if(*wstr_cur != ' ' && *wstr_cur != '\r' && *wstr_cur != '\n' && *wstr_cur != '\t') {
            wstr_last = wstr_cur;
        }
    }
    *++wstr_last = '\0';
    return wstr;
}

int strtotime(wchar_t * str, SYSTEMTIME *time) {
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int second = 0;
    int minute = 0;
    if(!str) {
        return FALSE;
    }
    _stscanf_s(str, L"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    time->wYear = year;
    time->wMonth = month;
    time->wDay = day;
    time->wHour = hour;
    time->wSecond = second;
    time->wMinute = minute;
    return TRUE;
}