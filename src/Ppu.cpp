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
        case 0xFF47: return bgp;
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
    case 0xFF47: bgp = value; break;    
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

// bgp is a lookup map for: "What the color ID actual color corresponds to?"
// Id only does not tells us the color, ID + bgp does
// 
// The map works like this:
// Bit 0 and 1 of BGP: What the ID 0 is mapped to? 
// Bit 2 and 3 of BGP: What the ID 1 is mapped to?
// Bit 4 and 5 of BGP: What the ID 2 is mapped to?
// Bit 6 and 7 of BGP: What the ID 3 is mapped to?

// If bgp is 0b00 00 00 00 -> Means: Every colorId of every background/window tile is mapped to 00 -> White
// If we change to 0b11 11 11 11 -> Means: Every colorId of every background/window tile is mapped to 11 -> Black
// If bgp is 0b00 11 00 00 -> Means: Only the Pixels with Color ID 2 are now mapped to 11 -> Black
// The Pixels with Color IDs 0, 1 and 3 are mapped to 00 -> White

uint8_t Ppu::backgroundShade (uint8_t colorId) const {
    return (bgp >> (colorId * 2) & 0b11);
}