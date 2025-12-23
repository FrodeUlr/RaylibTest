PROJECT_NAME = raylibtest

CC      = cc
SRC     = src/*.c
INCLUDES= -I./include
LIBS    = -L./lib -l:libraylib.a -lm
OUT     = ./bin/$(PROJECT_NAME)

.PHONY: all build run clean debug

all: build run

build:
	@mkdir -p bin
	@cp -r art bin/
	$(CC) -o $(OUT) $(INCLUDES) $(SRC) $(LIBS)

debug:
	@mkdir -p bin
	@cp -r art bin/
	$(CC) -g -o $(OUT) $(INCLUDES) $(SRC) $(LIBS)

run:
	@$(OUT)

clean:
	@rm -rf bin
