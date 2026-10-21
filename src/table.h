#ifndef TABLE_H
#define TABLE_H

#include "tinysql.h"
#include "heap.h"
#include "hash_index.h"
#include "avl_index.h"

typedef struct {
    HeapFile heap;
    HashIndex hidx;
    AVLIndex avl_gpa;
} Table;

Table* table_create(void);
void table_destroy(Table *t);
int table_insert(Table *t, Record r);
int table_delete(Table *t, int id);
Record* table_find_by_id(Table *t, int id);
int table_range_gpa(Table *t, float lo, float hi, int *out);
int table_select_dept(Table *t, const char *dept, int *out);
int table_load_csv(Table *t, const char *path);
void table_print_stats(const Table *t);

#endif