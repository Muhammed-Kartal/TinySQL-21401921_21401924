#ifndef HASH_INDEX_H
#define HASH_INDEX_H

#include "tinysql.h"

typedef struct HNode {
    int id;
    int slot;
    struct HNode *next;
} HNode;

typedef struct {
    HNode *buckets[TABLE_SIZE];
    int count;
    int collisions;
} HashIndex;

void hash_init(HashIndex *hi);
int hash_insert(HashIndex *hi, int id, int slot);
int hash_lookup(HashIndex *hi, int id);
int hash_delete(HashIndex *hi, int id);
int hash_update(HashIndex *hi, int id, int new_slot);
void hash_print_stats(const HashIndex *hi);
void hash_destroy(HashIndex *hi);

#endif