#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdint>

#include <filesystem>
#include <algorithm>

#include "../src/Bus.h"
#include "../src/Cpu.h"

namespace fs = std::filesystem;

void runRom(const fs::path& romPath) {
    std::ifstream romFile(romPath, std::ios::binary);

    if(!romFile) {
        std::fprintf(stderr, "Failed to open ROM: %s\n", romPath.string().c_str());
        return;
    }
    std::vector <uint8_t> rom;
    char byte;
    while(romFile.get(byte)) {
        rom.push_back(static_cast<uint8_t>(byte));
    }
    Bus bus(rom);
    Cpu cpu(bus);

    std::printf("\n===== %s =====\n", romPath.string().c_str());
    for (int i=0; i<100'000'000; i++) {
        int cycles = cpu.step();
        // if (i%1'000'000 == 0) printf("Executed %d operations \n", i);
        if (cycles == 0) break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <rom folder>\n", argv[0]);
        return 1;
    }
    
    fs::path folder = argv[1];

    if (!fs::is_directory(folder)) {
        std::fprintf(stderr, "Not a directory %s\n", argv[1]);
        return 1;
    }
    std::vector<fs::path> roms;
    
    for(const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".gb") 
            roms.push_back(entry.path());
    }

    std::sort(roms.begin(), roms.end());

    for (const auto& rom : roms) {
        runRom(rom);
    }

    return 0;
}
