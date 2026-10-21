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

clean:
	rm -f src/*.o $(TARGET)

.PHONY: clean debug
