OUT := out
BIN := gfxbin
SOURCES := src/*.c

always:
	mkdir -p out
build: always $(OUT)/$(BIN)

$(OUT)/$(BIN): $(SOURCES)
	gcc $^ -o $@ -lncurses

