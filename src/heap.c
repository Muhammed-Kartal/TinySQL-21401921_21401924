#include "heap.h"
#include <stdio.h>

void heap_init(HeapFile *hf) {
    (void)hf;
}

int heap_insert(HeapFile *hf, Record r) {
    (void)hf; (void)r;
    return -1;
}

Record* heap_search(HeapFile *hf, int id) {
    (void)hf; (void)id;
    return NULL;
}

int heap_delete(HeapFile *hf, int id) {
    (void)hf; (void)id;
    return -1;
}

void heap_load_csv(HeapFile *hf, const char *path) {
    (void)hf; (void)path;
}

void heap_print_stats(const HeapFile *hf) {
    (void)hf;
}