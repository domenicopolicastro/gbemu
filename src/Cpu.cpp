#include <cstdio>

#include "Cpu.h"

Cpu::Cpu(Bus& busData) : bus(busData) {}

uint16_t Cpu::getAF() const {
    return (static_cast<uint16_t>(a) << 8) | f;
}
uint16_t Cpu::getBC() const {
    return (static_cast<uint16_t>(b) << 8) | c;
}
uint16_t Cpu::getDE() const {
    return (static_cast<uint16_t>(d) << 8) | e;
}
uint16_t Cpu::getHL() const {
    return (static_cast<uint16_t>(h) << 8) | l;
}

void Cpu::setAF(uint16_t value) {
    a = static_cast<uint8_t>(value >> 8); 
    f = static_cast<uint8_t>(value & 0x00F0);
}
void Cpu::setBC(uint16_t value) {
    b = static_cast<uint8_t>(value >> 8); 
    c = static_cast<uint8_t>(value & 0x00FF);
}
void Cpu::setDE(uint16_t value) {
    d = static_cast<uint8_t>(value >> 8); 
    e = static_cast<uint8_t>(value & 0x00FF);
}
void Cpu::setHL(uint16_t value) {
    h = static_cast<uint8_t>(value >> 8); 
    l = static_cast<uint8_t>(value & 0x00FF);
}

uint8_t Cpu::readR8(uint8_t index) {
    switch (index) {
        case 0: return b;
        case 1: return c;
        case 2: return d;
        case 3: return e;
        case 4: return h;
        case 5: return l;
        case 6: return bus.read(getHL());
        case 7: return a;
    }
    return 0;
}
void Cpu::writeR8(uint8_t index, uint8_t value) {
    switch(index) {
        case 0: b = value; break;
        case 1: c = value; break;
        case 2: d = value; break;
        case 3: e = value; break;
        case 4: h = value; break;
        case 5: l = value; break;
        case 6: bus.write(getHL(), value); break;
        case 7: a = value; break;
    }
}
uint16_t Cpu::readR16(uint8_t index) {
    switch(index) {
        case 0: return getBC();
        case 1: return getDE();
        case 2: return getHL();
        case 3: return sp;
    }
    return 0;
}
void Cpu::writeR16(uint8_t index, uint16_t value) {
    switch(index) {
        case 0: setBC(value); break;
        case 1: setDE(value); break;
        case 2: setHL(value); break;
        case 3: sp = value; break;
    }
}

bool Cpu::getZeroFlag() const {
    return (f & 0x80) != 0;
}
bool Cpu::getSubtractFlag() const {
    return (f & 0x40) != 0;
}
bool Cpu::getHalfCarryFlag() const {
    return (f & 0x20) != 0;
}
bool Cpu::getCarryFlag() const {
    return (f & 0x10) != 0;
}

void Cpu::setZeroFlag(bool value) {
    if (value) f |= 0x80;
    else f &= 0x7F;
}
void Cpu::setSubtractFlag(bool value) {
    if (value) f |= 0x40;
    else f  &= 0xBF; 
}
void Cpu::setHalfCarryFlag(bool value) {
    if (value) f |= 0x20;
    else f  &= 0xDF;
}
void Cpu::setCarryFlag(bool value) {
    if (value) f |= 0x10;
    else f  &= 0xEF;
}

uint8_t Cpu::increment8(uint8_t value) {
    bool halfCarry = (value & 0x0F) == 0x0F;
    value++;
    setZeroFlag(value == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(halfCarry);
    return value;
}
uint8_t Cpu::decrement8(uint8_t value) {
    bool halfCarry = (value & 0x0F) == 0;
    value--;
    setZeroFlag(value == 0);
    setSubtractFlag(true);
    setHalfCarryFlag(halfCarry);
    return value;
}

void Cpu::add8(uint8_t operand) {
    bool halfCarry = (a & 0x0F) + (operand & 0x0F) > 0x0F;
    uint16_t result = static_cast<uint16_t>(a) + static_cast<uint16_t>(operand);
    bool carry = result > 0xFF;
    a = static_cast<uint8_t>(result);
    setZeroFlag(a == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(halfCarry);
    setCarryFlag(carry);
}
void Cpu::sub8(uint8_t operand) {
    bool carry = a < operand;
    bool halfCarry = (a & 0x0F) < (operand & 0x0F);
    a = a - operand;
    setZeroFlag(a == 0);
    setSubtractFlag(true);
    setHalfCarryFlag(halfCarry);
    setCarryFlag(carry);
}

void Cpu::and8(uint8_t operand) {
    a &= operand;
    setZeroFlag(a == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(true);
    setCarryFlag(false);
}
void Cpu::xor8(uint8_t operand) {
    a ^= operand;
    setZeroFlag(a == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(false);
    setCarryFlag(false);
}
void Cpu::or8(uint8_t operand) {
    a |= operand;
    setZeroFlag(a == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(false);
    setCarryFlag(false);
}

void Cpu::compare8(uint8_t operand) {
    bool carry = a < operand;
    bool halfCarry = (a & 0x0F) < (operand & 0x0F);
    uint8_t result = a - operand;
    setZeroFlag(result == 0);
    setSubtractFlag(true);
    setHalfCarryFlag(halfCarry);
    setCarryFlag(carry);
}
void Cpu::addWithCarry8(uint8_t operand) {
    uint8_t carryIn = getCarryFlag() ? 1 : 0;
    bool halfCarry = (a & 0x0F) + (operand & 0x0F) + carryIn > 0x0F;
    uint16_t result = static_cast<uint16_t>(a) + static_cast<uint16_t>(operand) + carryIn;
    bool carry = result > 0xFF;
    a = static_cast<uint8_t>(result);
    setZeroFlag(a == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(halfCarry);
    setCarryFlag(carry);
}
void Cpu::subWithCarry8(uint8_t operand) {
    uint8_t carryIn = getCarryFlag() ? 1 : 0;
    bool carry = static_cast<uint16_t>(a) < static_cast<uint16_t>(operand) + carryIn; 
    bool halfCarry = (a & 0x0F) < ((operand & 0x0F) + carryIn);
    a = a - operand - carryIn;
    setZeroFlag(a == 0);
    setSubtractFlag(true);
    setHalfCarryFlag(halfCarry);
    setCarryFlag(carry);
}

uint8_t Cpu::fetch8() {
    uint8_t value = bus.read(pc);
    pc++;
    return value;
}

uint16_t Cpu::fetch16() {
    uint8_t low = fetch8();
    uint8_t high = fetch8();
    return (static_cast<uint16_t>(high) << 8) | low;
}

void Cpu::push16(uint16_t value) {
    sp--;
    bus.write(sp, static_cast<uint8_t>(value >> 8));
    sp--;
    bus.write(sp, static_cast<uint8_t>(value));
}

uint16_t Cpu::pop16() {
    uint8_t low = bus.read(sp);
    sp++;
    uint8_t high = bus.read(sp);
    sp++;
    return (static_cast<uint16_t>(high) << 8 | low);
}

// 0xC1 1100 0001
// 0xD1 1101 0001
// 0xE1 1110 0001
// 0xF1 1111 0001

int Cpu::step() {
    uint8_t opcode = fetch8();

    if(opcode >= 0x40 && opcode <=0x7F) {
        if(opcode == 0x76) {
            // HALT - Not yet implemented
            return 4;
        } else {
            uint8_t destination = (opcode >> 3) & 0b111;
            uint8_t source = opcode & 0b111;
            uint8_t value = readR8(source);
            writeR8(destination, value);
            if(source == 6 || destination == 6) return 8;
            else return 4;
        }
    }
    if(opcode >= 0x80 && opcode <= 0xBF) {
        uint8_t operation = (opcode >> 3) & 0b111;
        uint8_t source = opcode & 0b111;
        uint8_t value = readR8(source);

        switch(operation) {
            case 0: add8(value); break;
            case 1: addWithCarry8(value); break;
            case 2: sub8(value); break;
            case 3: subWithCarry8(value); break;
            case 4: and8(value); break;
            case 5: xor8(value); break;
            case 6: or8(value); break;
            case 7: compare8(value); break;
        }
        return source == 6 ? 8 : 4;
    }
    if ((opcode & 0b11000111) == 0b00000110) {
        uint8_t destination = (opcode >> 3) & 0b111;
        uint8_t value = fetch8();
        writeR8(destination, value);
        return destination == 6 ? 12 : 8;
    }
    if ((opcode & 0b11000111) == 0b00000100) {
        uint8_t target = (opcode >> 3) & 0b111;
        uint8_t value = readR8(target);
        value = increment8(value);
        writeR8(target, value);
        return target == 6 ? 12 : 4;
    }
    if ((opcode & 0b11000111) == 0b00000101) {
        uint8_t target = (opcode >> 3) & 0b111;
        uint8_t value = readR8(target);
        value = decrement8(value);
        writeR8(target, value);
        return target == 6 ? 12 : 4;
    }
    if ((opcode & 0b11001111) == 0b00000001) {
        uint8_t target = (opcode >> 4) & 0b11;
        uint16_t value = fetch16();
        writeR16(target, value);
        return 12;
    }
    if ((opcode & 0b11100111) == 0b00100000) {
        uint8_t condition = (opcode >> 3) & 0b11;
        int8_t offset = static_cast<int8_t>(fetch8());
        bool jump;
        switch (condition) {
            case 0: jump = !getZeroFlag();  break;
            case 1: jump =  getZeroFlag();  break; 
            case 2: jump = !getCarryFlag(); break; 
            case 3: jump =  getCarryFlag(); break;
        }
        if (jump) {
            pc = pc + offset;
            return 12;
        }
        return 8;
    }
    if ((opcode & 0b11100111) == 0b11000010) {
        uint8_t condition = (opcode >> 3) & 0b11;
        uint16_t address = fetch16();
        bool jump;
        switch(condition) {
            case 0: jump = !getZeroFlag();  break;
            case 1: jump =  getZeroFlag();  break; 
            case 2: jump = !getCarryFlag(); break; 
            case 3: jump =  getCarryFlag(); break;
        }
        if (jump) {
            pc = address;
            return 16;
        }
        return 12;
    }
    if ((opcode & 0b11001111) == 0b11000101) {
        uint8_t target = (opcode >> 4) & 0b11;
        switch(target) {
            case 0: push16(getBC()); break;
            case 1: push16(getDE()); break;
            case 2: push16(getHL()); break;
            case 3: push16(getAF()); break;
        }
        return 16;
    }
    if ((opcode & 0b11001111) == 0b11000001) {
        uint8_t target = (opcode >> 4) & 0b11;
        switch(target) {
            case 0: setBC(pop16()); break;
            case 1: setDE(pop16()); break;
            case 2: setHL(pop16()); break;
            case 3: setAF(pop16()); break;
        }
        return 12;
    }
    switch(opcode) {
        case 0x00:
            return 4;
        case 0x18: {
            int8_t offset = static_cast<int8_t>(fetch8());
            pc += offset;
            return 12;
        }
        case 0xC3: {
            uint16_t address = fetch16();
            pc = address;
            return 16;
        }
        case 0xE9: 
            pc = getHL();
            return 4;
        
        default:
            std::fprintf(stderr, "Unhandled opcode 0x%02X, at PC 0x%04X.\n", opcode, pc-1);
            return 0;
    }
}

// Debug getters
uint16_t Cpu::getPC() const {
    return pc;
}
uint8_t Cpu::getA() const {
    return a;
}