#include "table.h"
#include <stdlib.h>

Table* table_create(void) {
    return NULL;
}

void table_destroy(Table *t) {
    (void)t;
}

int table_insert(Table *t, Record r) {
    (void)t; (void)r;
    return -1;
}

int table_delete(Table *t, int id) {
    (void)t; (void)id;
    return -1;
}

Record* table_find_by_id(Table *t, int id) {
    (void)t; (void)id;
    return NULL;
}

int table_range_gpa(Table *t, float lo, float hi, int *out) {
    (void)t; (void)lo; (void)hi; (void)out;
    return 0;
}

int table_select_dept(Table *t, const char *dept, int *out) {
    (void)t; (void)dept; (void)out;
    return 0;
}

int table_load_csv(Table *t, const char *path) {
    (void)t; (void)path;
    return -1;
}

void table_print_stats(const Table *t) {
    (void)t;
}