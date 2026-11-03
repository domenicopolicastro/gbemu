#include <cstdio>

#include "Ppu.h"

bool Ppu::tick(int cycles) {
    if ((lcdc & 0b10000000) == 0) return false;
    lineDots += cycles;
    if (lineDots >= 456) { 
        lineDots -= 456;
        ly++;
        if (ly == 154) ly = 0;
        if(ly == 144) return true;
    }
    return false;
}

uint8_t Ppu::read(uint16_t address) const {
    switch(address) {
        case 0xFF40: return lcdc;
        case 0xFF44: return ly;
        default: return 0xFF;
    }
}
void Ppu::write(uint16_t address, uint8_t value) {
    switch (address) {
    case 0xFF40:
        lcdc = value;
        if ((lcdc & 0b10000000) == 0) {
            ly = 0;
            lineDots = 0;
        }
        break;
    case 0xFF44: break;
    
    default:
        std::fprintf(stderr, "Unhandled write at 0x%04X\n", address);
        break;
    }
};

uint8_t Ppu::decodeTilePixel(uint8_t lowByte, uint8_t highByte, uint8_t x) const {
    uint8_t shift = 7 - x;
    uint8_t lowBit = (lowByte >> shift) & 1;
    uint8_t highBit = (highByte >> shift) & 1;
    return (highBit << 1) + lowBit;
}

DecodedTile Ppu::decodedTile(const std::array<uint8_t, 16>& tileData) const {
    DecodedTile decoded {};
    uint8_t x = 0;
    uint8_t row = 0;
    for (int i = 0; i < tileData.size(); i=i+2) {
        row = i / 2;
        while (x < 8) {
            decoded[row][x] = decodeTilePixel(tileData[i], tileData[i+1], x);
            x++;
        }
        x = 0;
    }
    return decoded;
}