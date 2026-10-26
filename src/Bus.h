#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>

#include "Timer.h"
#include "Ppu.h"

class Bus {
    private:
        const std::vector<uint8_t>& rom;
        std::array<uint8_t, 0x2000> vram{};
        std::array<uint8_t, 0x2000> wram{};
        std::array<uint8_t, 0xA0> oam{};
        std::array<uint8_t, 0x7F> hram{};
        uint8_t interruptEnable{};
        uint8_t interruptFlag{};

        uint8_t serialData = 0;

        Timer timer{};
        Ppu ppu{};

        std::string serialOutput;

    public:
        Bus(const std::vector<uint8_t>& romData);
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);

        void tick(int cycles);
        const std::string& getSerialDataOutput() const;
};