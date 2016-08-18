CFLAGS=-O3
LDFLAGS=-lncurses
SRC=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ=$(patsubst src/%.c, build/obj/%.o, $(SRC))

all: build/game
build/game: $(OBJ)
	gcc $(OBJ) $(LDFLAGS) -o $@
build/obj/%.o: src/%.c
	mkdir -p $(dir $@) && gcc $(CFLAGS) -c $< -o $@
clean:
	rm -rf build
