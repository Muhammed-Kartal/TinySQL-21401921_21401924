#include <stdio.h>
#include "table.h"
#include "query.h"

int main(void) {
    Table *t = table_create();
    if (t == NULL) {
        fprintf(stderr, "Tablo olusturulamadi\n");
        return 1;
    }
    repl(t);
    table_destroy(t);
    return 0;
}