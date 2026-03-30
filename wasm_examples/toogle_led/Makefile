# Variables
SRC = toggle.c
TARGET = toggle.wasm
OPT_TARGET = toggle_tiny.wasm
HEADER = toggle_wasm.h

# Tools
CC = clang
WASM_OPT = wasm-opt
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
         -Xlinker --export=toggle \
         -Xlinker --import-memory

.PHONY: all clean check

all: $(HEADER) check

$(OPT_TARGET): $(TARGET)
	$(WASM_OPT) $(TARGET) -Oz \
	--strip-debug \
	--strip-producers \
	--strip-target-features \
	-o $(OPT_TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

$(HEADER): $(OPT_TARGET)
	xxd -i $(OPT_TARGET) > $(HEADER)

check:
	@echo "--- Binary Size Report ---"
	@stat -c "Raw Wasm: %s bytes" $(TARGET)
	@stat -c "Optimized Wasm: %s bytes" $(OPT_TARGET)
	@echo "--------------------------"

clean:
	rm -f $(TARGET) $(OPT_TARGET) $(HEADER)
