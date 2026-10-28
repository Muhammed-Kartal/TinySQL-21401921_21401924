CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = tinysql

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

clean:
	rm -f src/*.o $(TARGET) tools/gen_csv

.PHONY: clean debug data
