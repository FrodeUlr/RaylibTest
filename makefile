# Variables for build configuration
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
OUT_DEBUG       = $(DEBUG_FOLDER)/$(PROJECT_NAME)

# Setup OS-specific includes and libraries
# Assumes only windows and linux, if you want to add more OSes, you'll need to modify this section
ifeq ($(OS),Windows_NT)
  INCLUDES_OS=$(INCLUDES_WIN)
  LIBS=$(LIBS_WIN)
	BEAR=""
else
  INCLUDES_OS=$(INCLUDES_LINUX)
  LIBS=$(LIBS_LINUX)
	BEAR="bear --"
endif

.PHONY: all build run clean memcheck

all: build run

# Pass MODE=debug to make for a debug build, otherwise it defaults to release
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
		BEAR=$(BEAR); \
	fi; \
	mkdir -p $$FOLDER; \
	cp -r art $$FOLDER/; \
	cp -r levels $$FOLDER/; \
	$$BEAR $(CC) $$FLAGS -o $$OUT $(INCLUDES) $(INCLUDES_OS) $(SRC) $(LIBS)

run:
	@$(OUT_RELEASE)

# Remove output directories and their contents
clean:
	@rm -rf bin

# Memory check using valgrind, only for debug builds
# Used to check for memory leaks and other memory-related issues
memcheck:
	make build MODE=debug
	@valgrind --leak-check=full --track-origins=yes $(OUT_DEBUG)
