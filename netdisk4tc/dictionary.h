#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <stdlib.h>
#include <Windows.h>

typedef struct _pair {
	WCHAR *key;
	void *value;
	struct _pair *previous;
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
typedef INT (*DictionaryEnumerator)(WCHAR *, void *);

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
DICTIONARY * InitializeDictionary(void);

//************************************
// Method:    DictionaryRemoveElement
// FullName:  DictionaryRemoveElement
// Access:    public 
// Returns:   void *，如果给定的键名未找到则返回NULL，否则从字典中删除给定键名的条目，并返回指向此条目的指针
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: WCHAR *
// Description: 按照键名移除指定字典的特定条目
//************************************
void * DictionaryRemoveElement(DICTIONARY *, WCHAR *);

//************************************
// Method:    DictionaryGetElement
// FullName:  DictionaryGetElement
// Access:    public 
// Returns:   void *
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: WCHAR *
// Description: 按照键名返回指定字典的特定条目
//************************************
void * DictionaryGetElement(DICTIONARY *, WCHAR *);

//************************************
// Method:    DictionarySetElement
// FullName:  DictionarySetElement
// Access:    public 
// Returns:   void *，如果给定的键名未找到则返回NULL，否则从字典中更新给定键名的条目，并返回指向此条目原始的值
// Qualifier:
// Parameter: DICTIONARY *
// Parameter: WCHAR *
// Description: 按照键名更新指定字典的特定条目
//************************************
void * DictionarySetElement(DICTIONARY *, WCHAR *, void *);

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
void DictionaryTraverse(const DICTIONARY *, DictionaryEnumerator);

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
void FreeDictionary(DICTIONARY *, int);

#endif