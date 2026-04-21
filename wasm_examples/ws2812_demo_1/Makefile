# Variables
SRC = demo.c
TARGET = demo.wasm

# Tools
CC = clang
XXD = xxd

# Flags:
# --target=wasm32: Target Wasm
# -nostdlib: Don't include bulky standard C libraries
# -Wl,--no-entry: No main() function needed
# -Wl,--export-all: Export your C functions to the Wasm engine
# -Os: Optimize for size
#CFLAGS = --target=wasm32 -O3 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -Wl,--strip-all -Wl,--gc-sections
CFLAGS = --target=wasm32 -Oz -nostdlib \
         -Wl,--no-entry \
         -Wl,--export=toggle \
         -Wl,--allow-undefined \
         -Wl,--strip-all \
         -Wl,--gc-sections \
         -Xlinker --no-entry \
         -Xlinker --export=demo \
         -Xlinker --import-memory

.PHONY: all clean check

all: $(TARGET) check

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

check:
	@echo "--- Binary Size Report ---"
	@stat -c "Raw Wasm: %s bytes" $(TARGET)
	@echo "--------------------------"

clean:
	rm -f $(TARGET) $(HEADER)
