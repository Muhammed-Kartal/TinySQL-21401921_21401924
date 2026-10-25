#include "hash_index.h"
#include <stdlib.h>
#include <stdio.h>


static int hash_fn(int id) {
    unsigned int k = (unsigned int)id;
    k = ((k >> 16) ^ k) * 0x45d9f3b;
    k = ((k >> 16) ^ k) * 0x45d9f3b;
    k = (k >> 16) ^ k;
    return (int)(k % TABLE_SIZE);
}


void hash_init(HashIndex *hi) {
    for (int i = 0; i < TABLE_SIZE; i++) hi->buckets[i] = NULL;
    hi->count = 0;
    hi->collisions = 0;
}

int hash_insert(HashIndex *hi, int id, int slot) {
    if (hash_lookup(hi, id) != -1) return -1; 
    int idx = hash_fn(id);
    if (hi->buckets[idx] != NULL) hi->collisions++;
    
    HNode *n = malloc(sizeof(HNode));
    n->id = id; n->slot = slot;
    n->next = hi->buckets[idx];
    hi->buckets[idx] = n;
    hi->count++;
    return 0;
}

int hash_lookup(HashIndex *hi, int id) {
    int idx = hash_fn(id);
    for (HNode *n = hi->buckets[idx]; n != NULL; n = n->next) {
        if (n->id == id) return n->slot;
    }
    return -1;
}

int hash_delete(HashIndex *hi, int id) {
    int idx = hash_fn(id);
    HNode *prev = NULL, *curr = hi->buckets[idx];
    while (curr != NULL) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else hi->buckets[idx] = curr->next;
            free(curr);
            hi->count--;
            return 0;
        }
        prev = curr; curr = curr->next;
    }
    return -1;
}

int hash_update(HashIndex *hi, int id, int new_slot) {
    int idx = hash_fn(id);
    for (HNode *n = hi->buckets[idx]; n != NULL; n = n->next) {
        if (n->id == id) {
            n->slot = new_slot;
            return 0;
        }
    }
    return -1;
}

void hash_print_stats(const HashIndex *hi) {
    printf("Hash Yük Faktörü: %.2f\n", (float)hi->count / TABLE_SIZE);
    printf("Hash Çakışmaları: %d\n", hi->collisions);
}

void hash_destroy(HashIndex *hi) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HNode *curr = hi->buckets[i];
        while (curr != NULL) {
            HNode *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}
