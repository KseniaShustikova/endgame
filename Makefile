CC = clang
EXECUTABLE = endgame

CFLAGS = `sdl2-config --libs --cflags` -std=c11 -Wall -Wextra -Werror -Wpedantic -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -g 

SOURCES := $(wildcard src/*.c)
OBJECTS = $(addprefix obj/,$(notdir $(SOURCES:.c=.o)))

all: objdir $(SOURCES) $(EXECUTABLE)

objdir:
	mkdir -p obj

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

obj/%.o: src/%.c
	clang -std=c11 -Wall -Wextra -Werror -Wpedantic -g -c -o $@ $<

uninstall: clean
	rm -rf $(EXECUTABLE)

clean:

reinstall: uninstall all
