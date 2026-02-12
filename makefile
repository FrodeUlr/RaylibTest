PROJECT_NAME = raylibtest

CC              = cc
SRC             = src/*.c
INCLUDES        = -I./include
INCLUDES_LINUX  = -I./raylib/linux/include
INCLUDES_WIN    = -I./raylib/win/include
LIBS_LINUX      = -L./raylib/linux/lib -l:libraylib.a -lm
LIBS_WIN        = -L./raylib/win/lib -lraylib -lopengl32 -lgdi32 -lwinmm
RELEASE_FOLDER  = ./bin/release
DEBUG_FOLDER    = ./bin/debug
OUT_RELEASE     = $(RELEASE_FOLDER)/$(PROJECT_NAME)
OUT_WIN         = $(RELEASE_FOLDER)/$(PROJECT_NAME).exe
OUT_DEBUG       = $(DEBUG_FOLDER)/$(PROJECT_NAME)

.PHONY: all build run clean memcheck windows

all: build run

build:
	@if [ "$(MODE)" = "debug" ]; then \
		FOLDER=$(DEBUG_FOLDER); \
		OUT=$(OUT_DEBUG); \
		FLAGS="-g"; \
		BEAR=""; \
	else \
		FOLDER=$(RELEASE_FOLDER); \
		OUT=$(OUT_RELEASE); \
		FLAGS=""; \
		BEAR="bear --"; \
	fi; \
	mkdir -p $$FOLDER; \
	cp -r art $$FOLDER/; \
	cp -r levels $$FOLDER/; \
	$$BEAR $(CC) $$FLAGS -o $$OUT $(INCLUDES) $(INCLUDES_LINUX) $(SRC) $(LIBS_LINUX)

windows:
	x86_64-w64-mingw32-gcc -o $(OUT_WIN) $(INCLUDES) $(INCLUDES_WIN) $(SRC) $(LIBS_WIN)

run:
	@$(OUT_RELEASE)

clean:
	@rm -rf bin

memcheck:
	make build MODE=debug
	@valgrind --leak-check=full --track-origins=yes $(OUT_DEBUG)
