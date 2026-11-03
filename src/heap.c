#include "heap.h"
#include <stdio.h>
#include <string.h>

/* complexity: O(1) */
void heap_init(HeapFile *hf) {
    hf->count = 0;
}
/* complexity: O(1) */
int heap_insert(HeapFile *hf, Record r) {
    if (hf->count >= MAX_RECORDS) return -1;
    hf->records[hf->count] = r;
    return hf->count++;
}

/* complexity: O(n) */
Record* heap_search(HeapFile *hf, int id) {
    for (int i = 0; i < hf->count; i++) {
        if (hf->records[i].id == id) return &hf->records[i];
    }
    return NULL;
}

/* complexity: O(n) */
int heap_delete(HeapFile *hf, int id) {
    for (int i = 0; i < hf->count; i++) {
        if (hf->records[i].id == id) {
            hf->records[i] = hf->records[hf->count - 1];
            hf->count--;
            return i; 
        }
    }
    return -1;
}

/* complexity: O(n) */
void heap_load_csv(HeapFile *hf, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Record r;
        if (sscanf(line, "%d,%63[^,],%31[^,],%f,%d", &r.id, r.name, r.dept, &r.gpa, &r.year) == 5) {
            heap_insert(hf, r);
        }
    }
    fclose(f);
}

/* complexity: O(1) */
void heap_print_stats(const HeapFile *hf) {
    printf("Toplam kayıt       : %d / %d\n", hf->count, MAX_RECORDS);
}
