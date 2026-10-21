#include "query.h"

void query_parse(const char *line, Query *q) {
    (void)line;
    q->type = Q_ERROR;
}

void query_exec(Table *t, const Query *q) {
    (void)t; (void)q;
}

void repl(Table *t) {
    (void)t;
}