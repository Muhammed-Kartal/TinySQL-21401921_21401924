#include <stdio.h>
#include <stdlib.h>

#define ID_MIN 1000
#define ID_MAX 99999

static unsigned int seed = 42;

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

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 10000;
    const char *path = argc > 2 ? argv[2] : "data/students.csv";
    int pool = ID_MAX - ID_MIN + 1;

    if (n <= 0 || n > pool) {
        fprintf(stderr, "Kayıt sayısı 1 ile %d arasında olmalı\n", pool);
        return 1;
    }

    const char *first[] = {"Ahmet", "Ayşe", "Mehmet", "Zeynep", "Mustafa", "Elif",
                           "Can", "Deniz", "Emre", "Selin", "Burak", "Ece"};
    const char *last[] = {"Yılmaz", "Kaya", "Demir", "Şahin", "Çelik", "Yıldız",
                          "Aydın", "Öztürk", "Arslan", "Doğan"};
    const char *dept[] = {"Bilgisayar", "Endüstri", "Elektrik", "Matematik", "Fizik"};

    int *ids = malloc(sizeof(int) * pool);
    if (!ids) return 1;
    for (int i = 0; i < pool; i++) ids[i] = ID_MIN + i;
    for (int i = 0; i < n; i++) {
        int j = i + rand_range(pool - i);
        int tmp = ids[i]; ids[i] = ids[j]; ids[j] = tmp;
    }

    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "Dosya açılamadı: %s\n", path);
        free(ids);
        return 1;
    }
    fprintf(f, "id,name,dept,gpa,year\n");
    for (int i = 0; i < n; i++) {
        int fi = rand_range(12);
        int la = rand_range(10);
        int de = rand_range(5);
        int gp = rand_range(401);
        int yr = 1 + rand_range(4);
        fprintf(f, "%d,%s %s,%s,%.2f,%d\n", ids[i], first[fi], last[la], dept[de], gp / 100.0, yr);
    }
    fclose(f);
    free(ids);
    printf("%d kayıt yazıldı: %s\n", n, path);
    return 0;
}