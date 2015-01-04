CC=clang
CFLAGS = -Wall -Wextra -x c -g -O0 -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lGL -lGLEW -lGLU
TARGET = four

OUTDIR = dist
FILES = main.c helpers.c helpers.h

WATCH_FILES = $(FILES) Makefile shaders

all: clean build

build:
	@echo "Running task 'build'..."
	@if ! [ -d $(OUTDIR) ]; then mkdir $(OUTDIR); fi
	@$(CC) $(CFLAGS) $(LDFLAGS) $(FILES) -o $(OUTDIR)/$(TARGET)
	@cp -r shaders $(OUTDIR)/
	@echo

clean:
	@echo "Running task 'clean'..."
	rm -r $(OUTDIR)
	@echo

watch:
	@echo "Waiting for file change..."
	@while inotifywait -q -r -e create,close_write,move,delete,delete_self $(WATCH_FILES); do \
		time -f "make all took %E s" make all; \
		echo; \
	done

help:
	@echo "Makefile for this project."
	@echo "Use as make [all | clean | watch | build | help]"
	@echo
	@echo "Make targets:"
	@echo "\tall\tWill clean and rebuild the project. Note that running 'make' on its own will invoke this target."
	@echo "\tclean\tWill delete the 'dist' directory."
	@echo "\tbuild\tWill rebuild the project."
	@echo "\twatch\t[requires inotifywait] Will listen for changes as you work on the project and automatically rebuild it."
	@echo "\thelp\tDisplays this help text."
	@echo
