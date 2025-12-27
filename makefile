PROJECT_NAME = raylibtest

BEAR            = bear --
CC              = cc
SRC             = src/*.c
INCLUDES        = -I./include
LIBS            = -L./lib -l:libraylib.a -lm
RELEASE_FOLDER	= ./bin/release
DEBUG_FOLDER	  = ./bin/debug
OUT_RELEASE     = $(RELEASE_FOLDER)/$(PROJECT_NAME)
OUT_DEBUG       = $(DEBUG_FOLDER)/$(PROJECT_NAME)

.PHONY: all build run clean

all: build run

build:
	@if [ "$(MODE)" = "debug" ]; then \
		FOLDER=$(DEBUG_FOLDER); \
		OUT=$(OUT_DEBUG); \
		FLAGS="-g"; \
	else \
		FOLDER=$(RELEASE_FOLDER); \
		OUT=$(OUT_RELEASE); \
		FLAGS=""; \
	fi; \
	mkdir -p $$FOLDER; \
	cp -r art $$FOLDER/; \
	cp -r levels $$FOLDER/; \
	$(BEAR) $(CC) $$FLAGS -o $$OUT $(INCLUDES) $(SRC) $(LIBS)

run:
	@$(OUT_RELEASE)

clean:
	@rm -rf bin
