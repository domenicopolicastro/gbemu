#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdint>

#include "../src/Bus.h"
#include "../src/Cpu.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <rom.gb>\n", argv[0]);
        return 1;
    }
    std::ifstream romFile(argv[1], std::ios::binary);
    if (!romFile) {
        std::fprintf(stderr, "Failed to open ROM: %s\n", argv[1]);
        return 1;
    }
    std::vector<uint8_t> rom;
    char byte;
    while (romFile.get(byte)) {
        rom.push_back(static_cast<uint8_t>(byte));
    }

    Bus bus(rom);
    Cpu cpu(bus);

    for (int i=0; i<10000000; i++) {
        int cycles = cpu.step();
        printf("Executed %d operations \n", i);
        if (cycles == 0) break;
    }

    return 0;
}
