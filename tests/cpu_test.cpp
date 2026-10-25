#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

#include <filesystem>
#include <algorithm>

#include "../src/Bus.h"
#include "../src/Cpu.h"

namespace fs = std::filesystem;

bool runRom(const fs::path& romPath) {
    std::ifstream romFile(romPath, std::ios::binary);

    if(!romFile) {
        std::fprintf(stderr, "Failed to open ROM: %s\n", romPath.string().c_str());
        return false;
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
        if(cycles == 0) {
            std::fprintf(stderr, "CPU execution error\n");
            return false;
        }
        bus.tick(cycles);
        const std::string& output = bus.getSerialDataOutput();
        
        if(output.find("\nPassed\n") != std::string::npos) return true;
        if(!output.empty() && output.back() == '\n' && output.find("\nFailed") != std::string::npos) return false;
    }

    std::fprintf(stderr, "Instruction limit reached without a result\n");
    return false;
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

    if(roms.empty()) {
        std::fprintf(stderr, "No .gb roms found in the supplied folder\n");
        return 1;
    }

    std::size_t passed = 0;

    for (const auto& rom : roms) {
        const bool success = runRom(rom);
        std::printf(
            "\n[%s] %s\n",
            success ? "PASS" : "FAIL",
            rom.filename().string().c_str()
        );
        if(success) passed++;
    }

    std::printf("\n%zu / %zu ROMs passed\n", passed, roms.size());
    return passed == roms.size() ? 0 : 1;
}
