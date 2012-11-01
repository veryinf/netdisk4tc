#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "utility.h"

#define DICT_OK 0
#define DICT_FATAL 1
#define DICT_MISS 2

typedef struct _pair {
    wchar_t *key;
    size_t size;
    void *value;
    struct _pair *next;
} DICTPAIR;

//************************************
// Method:    DictionaryEnumerator
// FullName:  DictionaryEnumerator
// Access:    public 
// Returns:   INT，如果返回 0 将中止迭代，其他值忽略
// Parameter: (WCHAR *, void *)
// Description: 供字典遍历时的迭代回调函数，4个参数指明键名, 键值, 值的大小及自定义数据
//************************************
typedef int (*dict_enumerator)(const wchar_t *, const void *, size_t size, void **);

typedef struct _dictionary {
    DICTPAIR *first;
    size_t length;
} DICTIONARY;

//************************************
// Returns:   DICTIONARY *
// Description: 创建新的字典
//************************************
DICTIONARY * dict_initialize(void);

//************************************
// Returns:   int，成功删除返回 DICT_OK，给定的键名未找到则返回 DICT_MISS
// Description: 按照键名移除指定字典的特定条目
//************************************
int dict_remove_element(DICTIONARY *, wchar_t *);

//************************************
// Returns:   size_t 返回指定键名条目的大小，未命中返回 0
// Description: 按照键名获取指定条目大小
//************************************
size_t dict_get_element_size(DICTIONARY *, wchar_t *);

//************************************
// Returns:   int 成功返回 DICT_OK，给定的键名未找到则返回 DICT_MISS，否则返回 DICT_FAILED
// Description: 按照键名返回指定字典的特定条目
//************************************
int dict_get_element_s(DICTIONARY *, wchar_t *, void **, size_t);

//************************************
// Returns:   void * 数据使用完毕应释放
// Description: 按照键名返回指定字典的特定条目，使用后应释放内存。如果不存在，返回 NULL
//************************************
void * dict_get_element(DICTIONARY *, wchar_t *);

//************************************
// Returns:   int 成功返回 DICT_OK，否则返回 DICT_FAILED
// Description: 按照键名更新指定字典的特定条目
//************************************
int dict_set_element(DICTIONARY *, wchar_t *, const void *, size_t);

//************************************
// Returns:   void
// Description: 遍历指定的字典，回调函数的返回值指明是否需要中止遍历
//************************************
void dict_traverse(const DICTIONARY *, dict_enumerator, void **);

//************************************
// Returns:   void
// Description: 销毁指定的字典
//************************************
void dict_destory(DICTIONARY **);

#endif