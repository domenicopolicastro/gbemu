#include <cstdio>

#include "Bus.h"

Bus::Bus(const std::vector<uint8_t>& romData) : rom(romData) {}

uint8_t Bus::read(uint16_t address) const {
    if (address <= 0x7FFF) return rom[address];

    if (address >= 0x8000 && address <= 0x9FFF) return vram[address - 0x8000];
    if (address >= 0xC000 && address <= 0xDFFF) return wram[address - 0xC000];
    if (address >= 0xE000 && address <= 0xFDFF) return wram[address - 0xE000];
    if (address >= 0xFE00 && address <= 0xFE9F) return oam[address - 0xFE00];
    if (address >= 0xFF80 && address <= 0xFFFE) return hram[address - 0xFF80];
    if (address == 0xFFFF) return interruptEnable;

    return 0xFF;
}

void Bus::write(uint16_t address, uint8_t value){
    if (address >= 0x8000 && address <= 0x9FFF) vram[address - 0x8000] = value;
    else if(address >= 0xC000 && address <= 0xDFFF) wram[address - 0xC000] = value;
    else if (address >= 0xE000 && address <= 0xFDFF) wram[address -0xE000] = value;
    else if (address >= 0xFE00 && address <= 0xFE9F) oam[address - 0xFE00] = value;
    else if (address >= 0xFF80 && address <= 0xFFFE) hram[address - 0xFF80] = value;
    else if (address == 0xFFFF) interruptEnable = value;
    else std::fprintf(stderr, "Unhandled write at 0x%04X\n", address);
}