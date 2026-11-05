#pragma once

#include <cstdint>
#include <array>

using DecodedTile = std::array<std::array<uint8_t, 8>, 8>;

class Ppu {
    private:
        uint16_t lineDots = 0;
        // This tells us what scanline we are currently processing;
        // row 0 is the top, row 143 is the bottom
        uint8_t ly = 0;
        uint8_t lcdc = 0x91;
        std::array<std::array<uint8_t, 160>, 144> frameBuffer {};

        uint8_t bgp = 0xFC; // Value after the boot
        uint8_t backgroundShade (uint8_t colorId) const;

        uint8_t decodeTilePixel (uint8_t lowByte, uint8_t highByte, uint8_t x) const;
        DecodedTile decodedTile(const std::array<uint8_t, 16>& tileData) const;

    public:
        bool tick(int cycles);
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);
};