#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define DICT_OK 0
#define DICT_FAILED 1
#define DICT_MISS 2

typedef struct _pair {
	wchar_t *key;
	void *value;
	size_t *size;
	struct _pair *next;
} DICTPAIR;

//************************************
// Method:    DictionaryEnumerator
// FullName:  DictionaryEnumerator
// Access:    public 
// Returns:   INT，如果返回 0 将中止迭代，其他值忽略
// Parameter: (WCHAR *, void *)
// Description: 供字典遍历时的迭代回调函数，2个参数指明键名及键值
//************************************
typedef int (*DictEnumerator)(wchar_t *, void *);

typedef struct _dictionary{
	DICTPAIR *first;
	DICTPAIR *last;
	unsigned int length;
} DICTIONARY;

//************************************
// Method:    InitializeDictionary
// FullName:  InitializeDictionary
// Access:    public 
// Returns:   DICTIONARY *
// Qualifier:
// Parameter: void
// Description: 创建新的字典
//************************************
DICTIONARY * InitializeDict(void);

//************************************
// Method:    DictionaryRemoveElement
// FullName:  DictionaryRemoveElement
// Access:    public 
// Returns:   int，成功删除返回 DICT_OK，给定的键名未找到则返回 DICT_MISS，否则返回 DICT_FAILED
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: WCHAR *
// Description: 按照键名移除指定字典的特定条目
//************************************
int DictRemoveElement(DICTIONARY *, wchar_t *);

//************************************
// Method:    DictGetElementSize
// FullName:  DictGetElementSize
// Access:    public 
// Returns:   size_t
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: wchar_t *
// Description: 按照键名获取指定条目大小
//************************************
size_t DictGetElementSize(DICTIONARY *, wchar_t *);

//************************************
// Method:    DictionaryGetElement
// FullName:  DictionaryGetElement
// Access:    public 
// Returns:   int 成功返回 DICT_OK，给定的键名未找到则返回 DICT_MISS，否则返回 DICT_FAILED
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: wchar_t *
// Parameter: void * * 
// Parameter: size_t
// Description: 按照键名返回指定字典的特定条目
//************************************
int DictGetElementS(DICTIONARY *, wchar_t *, void **, size_t);

//************************************
// Method:    DictGetElement
// FullName:  DictGetElement
// Access:    public 
// Returns:   void *
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: wchar_t *
// Description: 按照键名返回指定字典的特定条目，使用后应释放内存。如果不存在，返回 NULL
//************************************
void * DictGetElement(DICTIONARY *, wchar_t *);

//************************************
// Method:    DictionarySetElement
// FullName:  DictionarySetElement
// Access:    public 
// Returns:   int 成功返回 DICT_OK，给定的键名未找到则返回 DICT_MISS，否则返回 DICT_FAILED
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: WCHAR *
// Parameter: const void *
// Parameter: size_t
// Description: 按照键名更新指定字典的特定条目
//************************************
int DictSetElement(DICTIONARY *, wchar_t *, const void *, size_t);

//************************************
// Method:    DictionaryTraverse
// FullName:  DictionaryTraverse
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const DICTIONARY *
// Parameter: DictionaryEnumerator
// Description: 遍历指定的字典，回调函数的返回值指明是否需要中止遍历
//************************************
void DictTraverse(const DICTIONARY *, DictEnumerator);

//************************************
// Method:    FreeDictionary
// FullName:  FreeDictionary
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: BOOL 是否要同时销毁指向条目的指针（这些数据不是由字典所创建），除非你特别明白，否则不要选择将此参数设置为TRUE
// Description: 销毁指定的字典
//************************************
void FreeDict(DICTIONARY *);

#endif