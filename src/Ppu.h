#pragma once
#include <cstdint>

class Ppu {
    private:
        uint16_t lineDots = 0;
        // This tells us what scanline we are currently processing;
        // row 0 is the top, row 143 is the bottom
        uint8_t ly = 0;
        uint8_t lcdc = 0x91;
    public:
        bool tick(int cycles);
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);
};