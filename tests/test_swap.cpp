// Playground for the SWAP instruction.
// I wanted to verify that both shifts use the original value
// before the final result is assigned back to value.

#include <cstdio>
#include <cstdint>

int main () {
    uint8_t value = 0xC3;
    value = (value << 4) | (value >> 4);
    printf("New value %02X", value);
    return 0;
}