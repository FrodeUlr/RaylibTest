PROJECT_NAME = raylibtest

.PHONY: all build run clean

all: build run

build:
	@mkdir -p bin
	@cp -r art bin/
	@cc -o ./bin/raylibtest -I./include src/*.c -L./lib -l:libraylib.a -lm

run:
	@./bin/raylibtest

clean:
	@rm -rf bin
