#pragma once

#include <stdbool.h>
#include <stddef.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// 约定:
// 1. 正常存储的key, value都不为NULL
// 2. hash表传入的key是字符串(char *类型)由调用者自行管理,
// hash表内部实现通过strdup()复制一份拷贝.
// 3. hash表传入的value是指针, 都是经过malloc产生的.
// 4. 在hash表destroy的时候会执行free(key), free(value).

// Power of 2
#define INIT_CAPACITY 16

// Hash表entry
typedef struct {
    const char *key;
    void *value;
} ht_entry;

typedef struct htable {
    ht_entry *entries;
    unsigned int capacity;
    unsigned int length;
} htable_t;

htable_t *ht_create();

void ht_destroy(htable_t *table);

void *ht_get(htable_t *table, const char *key);

int ht_set(htable_t *table, const char *key, void *value);

int ht_del(htable_t *table, const char *key);

unsigned int ht_length(htable_t *table);

typedef struct {
    // 和ht_entry结构体保持一致
    struct {
        const char *key;
        void *value;
    };
    // Private
    htable_t *_table;
    unsigned int _index;
} hti; // hash表迭代器

static inline hti ht_iterator(htable_t *htable) {
    hti iter;
    iter._table = htable;
    iter._index = 0;

    return iter;
}

static inline bool ht_next(hti *iter) {
    htable_t *htable;
    unsigned int i;
    ht_entry entry;

    htable = iter->_table;
    while (iter->_index < htable->capacity) {
        i = iter->_index;
        iter->_index++;
        if (htable->entries[i].key != NULL) {
            entry = htable->entries[i];
            iter->key = entry.key;
            iter->value = entry.value;
            return true;
        }
    }

    return false;
}

/*
 * @iter: 一个hti迭代器
 * @htable_ptr: hash表指针
 */
#define ht_for_each(iter, htable_ptr)                                          \
    iter = ht_iterator(htable_ptr);                                            \
    while (ht_next(&iter))
