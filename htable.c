#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"

htable_t *ht_create() {
    htable_t *htable = malloc(sizeof(htable_t));
    assert(htable);

    *htable = (htable_t){
        .length = 0,
        .capacity = INIT_CAPACITY,
    };
    htable->entries = calloc(htable->capacity, sizeof(ht_entry));
    assert(htable->entries);

    return htable;
}

void ht_destroy(htable_t *htable) {
    for (int i = 0; i < htable->capacity; i++) {
        free((void *)htable->entries[i].key);
        free(htable->entries[i].value);
    }
    free(htable->entries);
    free(htable);
}

static uint64_t hash_key(const char *key) {
    uint64_t hash = FNV_OFFSET;
    for (const char *ch = key; *ch; ch++) {
        hash ^= (uint64_t)(*ch);
        hash *= FNV_PRIME;
    }
    return hash;
}

void *ht_get(htable_t *htable, const char *key) {
    uint64_t hash;
    unsigned int index;

    hash = hash_key(key);
    index = (hash & (htable->capacity - 1));

    while (htable->entries[index].key != NULL) {
        if (!strcmp(key, htable->entries[index].key)) {
            return htable->entries[index].value;
        }

        // Key isn't in current slot, move to next.(linear probing)
        index++;
        if (index >= htable->capacity) {
            index = 0;
        }
    }

    return NULL;
}

static int ht_set_entry(ht_entry *entries, unsigned int capacity,
                        const char *key, void *value, unsigned int *length) {
    uint64_t hash;
    unsigned int index;
    const char *key_copy;

    hash = hash_key(key);
    index = hash & (capacity - 1);

    while (entries[index].key != NULL) {
        // key已经存在, 更新key. 然后返回. 没有strdup()步骤.
        if (!strcmp(key, entries[index].value)) {
            entries[index].value = value;
            return 0;
        }

        // Key isn't in current slot, move to next.(linear probing)
        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    key_copy = strdup(key);
    assert(key_copy);

    (*length)++;

    entries[index].key = key_copy;
    entries[index].value = value;

    return 0;
    // TODO(chen): 异常时返回-1
}

static int ht_expand(htable_t *htable) {
    unsigned int new_capacity;
    ht_entry *new_entries;
    ht_entry entry;
    unsigned int length;

    length = 0;
    new_capacity = htable->capacity * 2;
    assert(new_capacity > htable->capacity); // no overflow

    new_entries = calloc(new_capacity, sizeof(ht_entry));
    assert(new_entries);

    for (int i = 0; i < htable->capacity; i++) {
        entry = htable->entries[i];
        if (entry.key) {
            ht_set_entry(new_entries, new_capacity, entry.key, entry.value,
                         &length);
        }
    }

    assert(length == htable->length);

    // Free old entries
    free(htable->entries);
    htable->entries = new_entries;
    htable->capacity = new_capacity;

    return 0;
    // TODO(chen): 异常的时候返回-1
}

int ht_set(htable_t *htable, const char *key, void *value) {
    int ret;

    assert(key != NULL);
    assert(value != NULL);

    if (htable->length >= htable->capacity * 0.75) {
        ret = ht_expand(htable);
        assert(ret == 0);
    }

    return ht_set_entry(htable->entries, htable->capacity, key, value,
                        &htable->length);
}

int ht_del(htable_t *htable, const char *key) {
    // 为了实现简单, 不进行哈希表缩容操作
    // 当然, 当存在很多key-value删除操作时, 需要认真考虑缩容步骤.
    uint64_t hash;
    unsigned int index;

    assert(key != NULL);
    hash = hash_key(key);
    index = (hash & (htable->capacity - 1));

    while (htable->entries[index].key != NULL) {
        if (!strcmp(key, htable->entries[index].key)) {
            free(htable->entries[index].value);
            htable->entries[index].value = NULL;
            free((void *)htable->entries[index].key);
            htable->entries[index].key = NULL;

            htable->length--;
            break;
        }

        // Key isn't in current slot, move to next.(linear probing)
        index++;
        if (index >= htable->capacity) {
            index = 0;
        }
    }
    return 0;
}

unsigned int ht_length(htable_t *htable) {
    return htable->length;
}
