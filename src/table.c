#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Table* table_create(void) {
    Table *t = malloc(sizeof(Table));
    if (!t) return NULL;
    heap_init(&t->heap);
    hash_init(&t->hidx);
    t->avl_gpa.root = NULL;
    t->avl_gpa.count = 0;
    return t;
}

void table_destroy(Table *t) {
    hash_destroy(&t->hidx);
    avl_destroy(t->avl_gpa.root);
    free(t);
}

int table_insert(Table *t, Record r) {
    if (t->heap.count >= MAX_RECORDS) return -2;
    if (hash_lookup(&t->hidx, r.id) != -1) return -1;
    
    int slot = heap_insert(&t->heap, r);
    hash_insert(&t->hidx, r.id, slot);
    t->avl_gpa.root = avl_insert(t->avl_gpa.root, r.gpa, r.id);
    t->avl_gpa.count++;
    return 0;
}

int table_delete(Table *t, int id) {
    int slot = hash_lookup(&t->hidx, id);
    if (slot == -1) return -1;

    Record r = t->heap.records[slot];
    t->avl_gpa.root = avl_delete(t->avl_gpa.root, r.gpa, id);
    t->avl_gpa.count--;

    int freed_slot = heap_delete(&t->heap, id);
    if (freed_slot < t->heap.count) {
        int moved_id = t->heap.records[freed_slot].id;
        hash_update(&t->hidx, moved_id, freed_slot);
    }
    hash_delete(&t->hidx, id);
    return 0;
}

Record* table_find_by_id(Table *t, int id) {
    int slot = hash_lookup(&t->hidx, id);
    if (slot != -1) return &t->heap.records[slot];
    return NULL;
}

int table_range_gpa(Table *t, float lo, float hi, int *out) {
    int cnt = 0;
    avl_range(t->avl_gpa.root, lo, hi, out, &cnt);
    return cnt;
}

int table_select_dept(Table *t, const char *dept, int *out) {
    int cnt = 0;
    for (int i = 0; i < t->heap.count; i++) {
        if (strcmp(t->heap.records[i].dept, dept) == 0) {
            out[cnt++] = t->heap.records[i].id;
        }
    }
    return cnt;
}

int table_load_csv(Table *t, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[256];
    int added = 0;
    while (fgets(line, sizeof(line), f)) {
        Record r;
        if (sscanf(line, "%d,%[^,],%[^,],%f,%d", &r.id, r.name, r.dept, &r.gpa, &r.year) == 5) {
            if (table_insert(t, r) == 0) added++;
        }
    }
    fclose(f);
    return added;
}

void table_print_stats(const Table *t) {
    printf("=== TinySQL İstatistikleri ===\n");
    heap_print_stats(&t->heap);
    hash_print_stats(&t->hidx);
    printf("AVL Yüksekliği: %d\n", avl_height(t->avl_gpa.root));
}
