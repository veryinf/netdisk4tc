#include "dictionary.h"


DICTIONARY * InitializeDictionary(void)
{
	DICTIONARY *dict = malloc(sizeof(DICTIONARY));
	memset(dict, 0, sizeof(DICTIONARY));
	dict->first = NULL;
	dict->last = NULL;
	dict->length = 0;
	return dict;
}

void * DictionarySetElement(DICTIONARY *pdict, wchar_t *key, const void *value, size_t size)
{
	void *ret = NULL;
	DICTPAIR *tmp, *nnode, *node = NULL;
	tmp = pdict->first;
	while(tmp != NULL) {
		if(wcscmp(key, tmp->key) == 0) {
			node = tmp;
			break;
		}
		tmp = tmp->next;
	}
	if(node != NULL) {
		ret = node->value;
		node->value = value;
	} else {
		nnode = malloc(sizeof(DICTPAIR));
		memset(nnode, 0, sizeof(DICTPAIR));
		nnode->key = key;
		nnode->value = malloc(size);
		memset(nnode->value, 0, size);
		memcpy(nnode->value, value, 1, size);
		nnode->next = NULL;
		nnode->previous = pdict->last;
		if(pdict->first == NULL) {
			pdict->first = nnode;
		} else {
			pdict->last->next = nnode;
		}
		pdict->last = nnode;
		pdict->length++;
	}
	return ret;
}

const void * DictionaryGetElement(DICTIONARY *pdict, WCHAR *key)
{
	void *ret = NULL;
	DICTPAIR *tmp = pdict->first;
	while(tmp != NULL) {
		if(wcscmp(key, tmp->key) == 0) {
			ret = tmp->value;
			break;
		}
		tmp = tmp->next;
	}
	return ret;
}

void * DictionaryRemoveElement(DICTIONARY *pdict, WCHAR *key)
{
	void *ret = NULL;
	DICTPAIR *tmp = pdict->first;
	while(tmp != NULL) {
		if(wcscmp(key, tmp->key) == 0) {
			ret = tmp->value;
			break;
		}
		tmp = tmp->next;
	}
	if(ret != NULL) {
		if(pdict->first == tmp) {
			tmp->next->previous = NULL;
			pdict->first = tmp->next;
		} else {
			tmp->previous->next = tmp->next;
			if(pdict->last == tmp) {
				pdict->last = tmp->previous;
			}
		}
		pdict->length--;
		free(tmp);
	}
	return ret;
}

void DictionaryTraverse(const DICTIONARY *pdict, DictionaryEnumerator enumerator)
{
	DICTPAIR *node = pdict->first;
	while(node != NULL) {
		if(!enumerator(node->key, node->value)) {
			break;
		}
		node = node->next;
	}
}

void FreeDictionary(DICTIONARY *pdict)
{
	DICTPAIR *node = pdict->first;
	while(node != NULL) {
		node = node->next;
		if(node != NULL) {
			free(node->previous->value);
			node->previous->value = NULL;
			free(node->previous);
		}
	}
	if(pdict->last != NULL) {
		free(pdict->last->value);
		pdict->last->value = NULL;
		free(pdict->last);
	}
	free(pdict);
	pdict = NULL;
}