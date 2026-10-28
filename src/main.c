#include <stdio.h>
#include <string.h>
#include "table.h"
#include "query.h"

int main(int argc, char *argv[]) {
    Table *t = table_create();
    if (t == NULL) {
        fprintf(stderr, "Tablo oluşturulamadı\n");
        return 1;
    }
    if (argc > 1 && strcmp(argv[1], "--echo") == 0) repl_set_echo(1);
    repl(t);
    table_destroy(t);
    return 0;
}