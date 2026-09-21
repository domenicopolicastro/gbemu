#include <cstdio>

#include "Timer.h"

uint8_t Timer::read(uint16_t address) const {
    switch (address) {
        case 0xFF04: return static_cast<uint8_t>(dividerCounter >> 8);
        case 0xFF05: return tima;
        case 0xFF06: return tma;
        case 0xFF07: return tac;
        default: 
            std::fprintf(stderr, 
                "Unhandled Timer read at address 0x%04X.\n", 
                static_cast<unsigned>(address));
            return 0xFF;
    }
}

void Timer::write(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF04: dividerCounter = 0; break;
        case 0xFF05: tima = value; break;
        case 0xFF06: tma = value; break;
        case 0xFF07: tac = value & 0b00000111; break;
        default: 
            std::fprintf(stderr, 
                "Unhandled Timer write at address 0x%04X.\n",
                static_cast<unsigned>(address));
            break;
    }
}