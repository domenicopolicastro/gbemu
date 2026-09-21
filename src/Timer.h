#pragma once

#include <cstdint>

class Timer {
    public:
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);

        bool tick(int cycles);
    private:
        uint16_t dividerCounter{};

        uint8_t tima{};
        uint8_t tma{};
        uint8_t tac{};

        uint8_t selectedDividerBit() const;
        bool timerSignal() const;

        bool incrementTima();
};