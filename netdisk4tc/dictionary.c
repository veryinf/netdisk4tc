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

void * DictionarySetElement(DICTIONARY *pdict, WCHAR *key, void *value)
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
		nnode->value = value;
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

void * DictionaryGetElement(DICTIONARY *pdict, WCHAR *key)
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

void FreeDictionary(DICTIONARY *pdict, BOOL autofree)
{
	DICTPAIR *node = pdict->first;
	while(node != NULL) {
		node = node->next;
		if(node != NULL) {
			if(autofree) {
				free(node->previous->value);
			}
			free(node->previous);
		}
	}
	if(pdict->last != NULL) {
		if(autofree) {
			free(pdict->last->value);
		}
		free(pdict->last);
	}
	free(pdict);
	pdict = NULL;
}