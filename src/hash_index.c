#include "hash_index.h"

void hash_init(HashIndex *hi) {
    (void)hi;
}

int hash_insert(HashIndex *hi, int id, int slot) {
    (void)hi; (void)id; (void)slot;
    return -1;
}

int hash_lookup(HashIndex *hi, int id) {
    (void)hi; (void)id;
    return -1;
}

int hash_delete(HashIndex *hi, int id) {
    (void)hi; (void)id;
    return -1;
}

int hash_update(HashIndex *hi, int id, int new_slot) {
    (void)hi; (void)id; (void)new_slot;
    return -1;
}

void hash_print_stats(const HashIndex *hi) {
    (void)hi;
}

void hash_destroy(HashIndex *hi) {
    (void)hi;
}