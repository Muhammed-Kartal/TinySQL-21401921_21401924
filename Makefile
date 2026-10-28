CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = tinysql
BENCH_OBJ = $(filter-out src/main.o,$(OBJ))

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

src/%.o: src/%.c src/*.h
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS = -Wall -Wextra -std=c99 -g -fsanitize=address
debug: clean $(TARGET)

data: tools/gen_csv
	./tools/gen_csv 10000 data/students.csv

tools/gen_csv: tools/gen_csv.c
	$(CC) $(CFLAGS) -o $@ $<

bench: tools/bench
	./tools/bench > report/olcumler.md
	cat report/olcumler.md

tools/bench: tools/bench.c $(BENCH_OBJ) src/*.h
	$(CC) $(CFLAGS) -Isrc -o $@ tools/bench.c $(BENCH_OBJ) -lm

clean:
	rm -f src/*.o $(TARGET) tools/gen_csv tools/bench

.PHONY: clean debug data bench
