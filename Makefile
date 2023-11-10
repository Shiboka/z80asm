CC=gcc
CFLAGS = -Wall -g
DEPS = src/asm.h src/util.h
OBJ = obj/main.o obj/asm.o obj/util.o

obj/%.o: src/%.c $(DEPS)
	mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	mkdir -p bin
	$(CC) -o bin/z80asm $(OBJ) -lm

clean:
	rm -f obj/*.o bin/*