#ifndef HEAP_H
#define HEAP_H

#include "tinysql.h"

#define MAX_RECORDS 100000

typedef struct {
    Record records[MAX_RECORDS];
    int count;
} HeapFile;

void heap_init(HeapFile *hf);
int heap_insert(HeapFile *hf, Record r);
Record* heap_search(HeapFile *hf, int id);
int heap_delete(HeapFile *hf, int id);
void heap_load_csv(HeapFile *hf, const char *path);
void heap_print_stats(const HeapFile *hf);

#endif