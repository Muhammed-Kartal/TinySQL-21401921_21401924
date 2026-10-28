#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "table.h"

#define ID_MIN 1000
#define ID_MAX 99999
#define NUM_QUERIES 1000

static unsigned int seed = 42;
static volatile long sink;

static unsigned int next_rand(void) {
    seed = seed * 1103515245u + 12345u;
    return (seed >> 16) & 0x7FFF;
}

static int rand_range(int n) {
    unsigned int high = next_rand();
    unsigned int low = next_rand();
    unsigned int r = (high << 15) | low;
    return (int)(r % (unsigned int)n);
}

static Record *make_records(int n) {
    int pool = ID_MAX - ID_MIN + 1;
    int *ids = malloc(sizeof(int) * pool);
    Record *recs = malloc(sizeof(Record) * n);
    for (int i = 0; i < pool; i++) ids[i] = ID_MIN + i;
    for (int i = 0; i < n; i++) {
        int j = i + rand_range(pool - i);
        int tmp = ids[i]; ids[i] = ids[j]; ids[j] = tmp;
        recs[i].id = ids[i];
        snprintf(recs[i].name, MAX_NAME, "Ogrenci %d", ids[i]);
        snprintf(recs[i].dept, MAX_DEPT, "Bolum %d", rand_range(5));
        recs[i].gpa = rand_range(401) / 100.0f;
        recs[i].year = 1 + rand_range(4);
    }
    free(ids);
    return recs;
}

static Table *build_table(const Record *recs, int n) {
    Table *t = table_create();
    for (int i = 0; i < n; i++) table_insert(t, recs[i]);
    return t;
}

static void pick_ids(const Record *recs, int n, int *out, int k) {
    for (int i = 0; i < k; i++) out[i] = recs[rand_range(n)].id;
}

static void run_heap(Table *t, const int *ids, int k) {
    for (int i = 0; i < k; i++) sink += heap_search(&t->heap, ids[i]) != NULL;
}

static void run_hash(Table *t, const int *ids, int k) {
    for (int i = 0; i < k; i++) sink += hash_lookup(&t->hidx, ids[i]);
}

static double ns_per_op(void (*fn)(Table *, const int *, int), Table *t, const int *ids, int k) {
    long reps = 0;
    double sec;
    clock_t start = clock();
    do {
        fn(t, ids, k);
        reps++;
        sec = (double)(clock() - start) / CLOCKS_PER_SEC;
    } while (sec < 0.1);
    return sec * 1e9 / ((double)reps * k);
}

static void table1(void) {
    int sizes[] = {100, 1000, 10000};
    int ids[NUM_QUERIES];
    printf("## Tablo 1: Nokta sorgusu (Kabul 1-5)\n\n");
    printf("%d rastgele mevcut id aranmıştır; süreler işlem başına ortalamadır.\n\n", NUM_QUERIES);
    printf("| n | Heap araması O(n) (ns) | Hash araması O(1) (ns) | Hızlanma |\n");
    printf("|---:|---:|---:|---:|\n");
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        Record *recs = make_records(n);
        Table *t = build_table(recs, n);
        pick_ids(recs, n, ids, NUM_QUERIES);
        double h = ns_per_op(run_heap, t, ids, NUM_QUERIES);
        double x = ns_per_op(run_hash, t, ids, NUM_QUERIES);
        printf("| %d | %.1f | %.1f | %.0fx |\n", n, h, x, h / x);
        table_destroy(t);
        free(recs);
    }
    printf("\n");
}

int main(void) {
    printf("# TinySQL Ölçüm Sonuçları\n\n");
    table1();
    return 0;
}