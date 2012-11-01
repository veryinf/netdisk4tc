#include "dictionary.h"


DICTIONARY * dict_initialize(void) {
    DICTIONARY *dict = (DICTIONARY *)malloc(sizeof(DICTIONARY));
    memset(dict, 0, sizeof(DICTIONARY));
    dict->first = NULL;
    dict->length = 0;
    return dict;
}

int dict_set_element(DICTIONARY *pdict, wchar_t *key, const void *value, size_t size) {
    void *ret = NULL;
    DICTPAIR *iter, *node = NULL;
    iter = pdict->first;
    while(iter != NULL) {
        if(wcscmp(key, iter->key) == 0) {
            node = iter;
        }
        if(iter->next == NULL) {
            break;
        }
        iter = iter->next;
    }
    if(node != NULL) {
        free(iter->value);
        iter->value = malloc(size);
        if(!iter->value) {
            return DICT_FATAL;
        }
        memcpy(iter->value, value, size);
        iter->size = size;
        return DICT_OK;
    } else {
        node = (DICTPAIR*)malloc(sizeof(DICTPAIR));
        memset(node, 0, sizeof(DICTPAIR));
        node->key = _wcsdup(key);
        if(!node->key) {
            return DICT_FATAL;
        }
        node->value = malloc(size);
        if(!node->value) {
            return DICT_FATAL;
        }
        memset(node->value, 0, size);
        memcpy(node->value, value, size);
        node->size = size;
        node->next = NULL;
        if(iter) {
            iter->next = node;
        } else {
            pdict->first = node;
        }
        pdict->length++;
        return DICT_OK;
    }
}

int dict_get_element_s(DICTIONARY *pdict, wchar_t *key, void **value, size_t size) {
    DICTPAIR *iter = pdict->first;
    while(iter != NULL) {
        if(wcscmp(key, iter->key) == 0) {
            *value = malloc(size);
            if (!*value) {
                return DICT_FATAL;
            }
            memcpy(*value, iter->value, size);
            return DICT_OK;
        }
        iter = iter->next;
    }
    return DICT_MISS;
}

void * dict_get_element(DICTIONARY *pdict, wchar_t *key) {
    void *ret = NULL;
    DICTPAIR *iter = pdict->first;
    while(iter != NULL) {
        if(wcscmp(key, iter->key) == 0) {
            ret = malloc(iter->size);
            if(!ret) {
                return NULL;
            }
            memcpy(ret, iter->value, iter->size);
            return ret;
        }
        iter = iter->next;
    }
    return NULL;
}

int dict_remove_element(DICTIONARY *pdict, wchar_t *key) {
    DICTPAIR *iter = pdict->first, *hit = NULL;
    while(iter != NULL) {
        if(pdict->first == iter && wcscmp(key, iter->key) == 0) {
            pdict->first = iter->next;
            hit = iter;
            break;
        }
        if(iter->next != NULL && wcscmp(key, iter->next->key) == 0) {
            hit = iter->next;
            iter->next = iter->next->next;
            break;
        }
        iter = iter->next;
    }
    if(hit) {
        free(hit->key);
        free(hit->value);
        free(hit);
        pdict->length--;
        return DICT_OK;
    }
    return DICT_MISS;
}

void dict_traverse(const DICTIONARY *pdict, dict_enumerator enumerator, void **data) {
    DICTPAIR *node = pdict->first;
    while(node != NULL) {
        if(!enumerator(node->key, node->value, node->size, data)) {
            break;
        }
        node = node->next;
    }
}

void dict_destory(DICTIONARY **pdict) {
    DICTPAIR *iter = (*pdict)->first, *tmp;
    while(iter != NULL) {
        tmp = iter;
        iter = iter->next;
        free(tmp->key);
        free(tmp->value);
    }
    free(*pdict);
    *pdict = NULL;
}

size_t dict_get_element_size(DICTIONARY *pdict, wchar_t *key) {
    DICTPAIR *iter = pdict->first;
    while(iter != NULL) {
        if(wcscmp(key, iter->key) == 0) {
            return iter->size;
        }
        iter = iter->next;
    }
    return 0;
}