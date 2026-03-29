// logic.c
// The "export" attribute tells the compiler to make this function visible to Wasm3
__attribute__((used))
int add(int a, int b) {
    return a + b;
}
