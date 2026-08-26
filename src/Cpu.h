#pragma once

#include <cstdint>

#include "Bus.h"

class Cpu {
    private:
        Bus& bus;

        uint8_t a = 0x01;
        uint8_t f = 0xB0;
        uint8_t b = 0x00;
        uint8_t c = 0x13;
        uint8_t d = 0x00;
        uint8_t e = 0xD8;
        uint8_t h = 0x01;
        uint8_t l = 0x4D;

        uint16_t pc = 0x0100;
        uint16_t sp = 0xFFFE;

        uint16_t getAF() const;
        uint16_t getBC() const;
        uint16_t getDE() const;
        uint16_t getHL() const;

        void setAF(uint16_t value);
        void setBC(uint16_t value);
        void setDE(uint16_t value);
        void setHL(uint16_t value);

        uint8_t readR8(uint8_t index);
        void writeR8(uint8_t index, uint8_t value);
        uint16_t readR16(uint8_t index);
        void writeR16(uint8_t index, uint16_t value);

        bool getZeroFlag() const;
        bool getSubtractFlag() const;
        bool getHalfCarryFlag() const;
        bool getCarryFlag() const;
        
        void setZeroFlag(bool value);
        void setSubtractFlag(bool value);
        void setHalfCarryFlag(bool value);
        void setCarryFlag(bool value);

        uint8_t increment8(uint8_t value);
        uint8_t decrement8(uint8_t value);

        void add8(uint8_t operand);
        void sub8(uint8_t operand);

        void and8(uint8_t operand);
        void xor8(uint8_t operand);
        void or8(uint8_t operand);

        void compare8(uint8_t operand);
        void addWithCarry8(uint8_t operand);
        void subWithCarry8(uint8_t operand);

        uint8_t fetch8();
        uint16_t fetch16();

        void push16(uint16_t value);
        uint16_t pop16();

    public:
        Cpu(Bus& busData);
        int step();

        // Debug getters
        uint16_t getPC() const;
        uint8_t getA() const;
};