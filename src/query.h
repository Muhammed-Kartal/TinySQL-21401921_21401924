#ifndef QUERY_H
#define QUERY_H

#include "tinysql.h"
#include "table.h"

typedef enum {
    Q_INSERT, Q_SELECT_ID, Q_SELECT_GPA_RANGE,
    Q_SELECT_DEPT, Q_DELETE, Q_STATS, Q_LOAD, Q_EXIT, Q_ERROR
} QueryType;

typedef struct {
    QueryType type;
    Record rec;
    int id;
    float lo, hi;
    char dept[MAX_DEPT];
    char path[256];
} Query;

void query_parse(const char *line, Query *q);
void query_exec(Table *t, const Query *q);
void repl(Table *t);

#endif