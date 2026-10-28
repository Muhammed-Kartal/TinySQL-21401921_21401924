#include "query.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static double last_query_time = 0.0;

void query_parse(const char *line, Query *q) {
    char cmd[32];
    q->type = Q_ERROR;
    while (*line == ' ' || *line == '\t') line++;
    if (sscanf(line, "%31s", cmd) != 1) return;

    if (strcmp(cmd, "INSERT") == 0) {
        if (sscanf(line, "INSERT %d \"%[^\"]\" \"%[^\"]\" %f %d", 
            &q->rec.id, q->rec.name, q->rec.dept, &q->rec.gpa, &q->rec.year) == 5) {
            q->type = Q_INSERT;
        }
    } else if (strcmp(cmd, "SELECT") == 0) {
        char field[32];
        sscanf(line, "SELECT %31s", field);
        if (strcmp(field, "id") == 0) {
            if (sscanf(line, "SELECT id = %d", &q->id) == 1) q->type = Q_SELECT_ID;
        } else if (strcmp(field, "gpa") == 0) {
            if (sscanf(line, "SELECT gpa BETWEEN %f %f", &q->lo, &q->hi) == 2) q->type = Q_SELECT_GPA_RANGE;
        } else if (strcmp(field, "dept") == 0) {
            if (sscanf(line, "SELECT dept = \"%[^\"]\"", q->dept) == 1) q->type = Q_SELECT_DEPT;
        }
    } else if (strcmp(cmd, "DELETE") == 0) {
        if (sscanf(line, "DELETE %d", &q->id) == 1) q->type = Q_DELETE;
    } else if (strcmp(cmd, "STATS") == 0) {
        q->type = Q_STATS;
    } else if (strcmp(cmd, "LOAD") == 0) {
        if (sscanf(line, "LOAD %255s", q->path) == 1) q->type = Q_LOAD;
    } else if (strcmp(cmd, "EXIT") == 0) {
        q->type = Q_EXIT;
    }
}

#define MAX_SHOW 10

static void print_record(const Record *r) {
    printf("  %d | %s | %s | %.2f | %d\n", r->id, r->name, r->dept, r->gpa, r->year);
}

static void print_results(Table *t, const int *out, int n) {
    printf("%d kayıt bulundu.\n", n);
    for (int i = 0; i < n && i < MAX_SHOW; i++) print_record(table_find_by_id(t, out[i]));
    if (n > MAX_SHOW) printf("  ... ve %d kayıt daha\n", n - MAX_SHOW);
}

static double elapsed_since(clock_t start) {
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

void query_exec(Table *t, const Query *q) {
    static int out[MAX_RECORDS];
    clock_t start = clock();
    int res;
    Record *r;

    switch (q->type) {
        case Q_INSERT:
            res = table_insert(t, q->rec);
            last_query_time = elapsed_since(start);
            if (res == 0) printf("Kayıt eklendi.\n");
            else if (res == -1) printf("Hata: Bu id zaten mevcut.\n");
            else printf("Hata: Kapasite dolu.\n");
            break;
        case Q_SELECT_ID:
            r = table_find_by_id(t, q->id);
            last_query_time = elapsed_since(start);
            if (r) { printf("1 kayıt bulundu.\n"); print_record(r); }
            else printf("Kayıt bulunamadı.\n");
            break;
        case Q_SELECT_GPA_RANGE:
            res = table_range_gpa(t, q->lo, q->hi, out);
            last_query_time = elapsed_since(start);
            print_results(t, out, res);
            break;
        case Q_SELECT_DEPT:
            res = table_select_dept(t, q->dept, out);
            last_query_time = elapsed_since(start);
            print_results(t, out, res);
            break;
        case Q_DELETE:
            res = table_delete(t, q->id);
            last_query_time = elapsed_since(start);
            if (res == 0) printf("Kayıt silindi.\n");
            else printf("Hata: Kayıt bulunamadı.\n");
            break;
        case Q_LOAD:
            res = table_load_csv(t, q->path);
            last_query_time = elapsed_since(start);
            if (res >= 0) printf("%d kayıt yüklendi.\n", res);
            else printf("Dosya açılamadı.\n");
            break;
        case Q_STATS:
            table_print_stats(t);
            printf("Son sorgu süresi   : %.6f s\n", last_query_time);
            return;
        default:
            printf("Geçersiz veya eksik komut.\n");
            return;
    }
    printf("Süre: %.6f s\n", last_query_time);
}

void repl(Table *t) {
    char line[256];
    Query q;
    printf("TinySQL REPL Başlatıldı.\n");
    while (1) {
        printf("tinysql> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        char first[2];
        if (sscanf(line, "%1s", first) != 1) continue;
        query_parse(line, &q);
        if (q.type == Q_EXIT) break;
        if (q.type == Q_ERROR) {
            printf("Sözdizimi hatası. (Örn: SELECT id = 1042, LOAD data/students.csv)\n");
            continue;
        }
        query_exec(t, &q);
    }
}
