#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "utility.h"

#define DICT_OK 0
#define DICT_FATAL 1
#define DICT_MISS 2

typedef void (*dict_destroyer)(void **);

typedef struct _pair {
    wchar_t *key;
    size_t size;
    void *value;
    dict_destroyer destroyer;
    struct _pair *next;
} DictEntry;

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
    DictEntry *first;
    size_t length;
} Dictionary;

//************************************
// Returns:   DICTIONARY *
// Description: 创建新的字典
//************************************
Dictionary * dict_initialize(void);

//************************************
// Returns:   int，成功删除返回 DICT_OK，给定的键名未找到则返回 DICT_MISS
// Description: 按照键名移除指定字典的特定条目
//************************************
int dict_remove_element(Dictionary *, wchar_t *);

//************************************
// Returns:   size_t 返回指定键名条目的大小，未命中返回 0
// Description: 按照键名获取指定条目大小
//************************************
size_t dict_get_element_size(Dictionary *, wchar_t *);

//************************************
// Returns:   int 成功返回 DICT_OK，给定的键名未找到则返回 DICT_MISS，否则返回 DICT_FAILED
// Description: 按照键名返回指定字典的特定条目(直接引用)
//************************************
int dict_get_element_s(Dictionary *, wchar_t *, const void **);

//************************************
// Returns:   void * 数据使用完毕应释放
// Description: 按照键名返回指定字典的特定条目的副本，使用后应释放内存。如果不存在，返回 NULL
//************************************
void * dict_get_element(Dictionary *, wchar_t *);

//************************************
// Returns:   int
// Description: 判断字典中是否存在指定键名
//************************************
int dict_exists(Dictionary *, wchar_t *);

//************************************
// Returns:   int 成功返回 DICT_OK，否则返回 DICT_FAILED
// Description: 按照键名更新指定字典的特定条目, 附加的参数说明如何释放给定的数据（如果给定的数据包含多级的分配内存较为有用）
//************************************
int dict_set_element_s(Dictionary *, wchar_t *, const void *, size_t, dict_destroyer);

//************************************
// Returns:   int 成功返回 DICT_OK，否则返回 DICT_FAILED
// Description: 按照键名更新指定字典的特定条目
//************************************
int dict_set_element(Dictionary *, wchar_t *, const void *, size_t);

//************************************
// Returns:   void
// Description: 遍历指定的字典，回调函数的返回值指明是否需要中止遍历
//************************************
void dict_traverse(const Dictionary *, dict_enumerator, void **);

//************************************
// Returns:   void
// Description: 销毁指定的字典
//************************************
void dict_destroy(Dictionary **);
#endif