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

void Cpu::increment8(uint8_t& reg) {
    bool halfCarry = (reg & 0x0F) == 0x0F;
    reg++;
    setZeroFlag(reg == 0);
    setSubtractFlag(false);
    setHalfCarryFlag(halfCarry);
}

void Cpu::decrement8(uint8_t& reg) {
    bool halfCarry = (reg & 0x0F) == 0;
    reg--;
    setZeroFlag(reg == 0);
    setSubtractFlag(true);
    setHalfCarryFlag(halfCarry);
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

int Cpu::step() {
    uint8_t opcode = fetch8();

    switch(opcode) {
        case 0x00:
            return 4;
        
        case 0x06:
            b = fetch8();
            return 8;
        case 0x0E:
            c = fetch8();
            return 8;
        case 0x16:
            d = fetch8();
            return 8;
        case 0x1E:
            e = fetch8();
            return 8;
        case 0x26:
            h = fetch8();
            return 8;
        case 0x2E:
            l = fetch8();
            return 8;
        case 0x36:
            bus.write(getHL(), fetch8());
            return 12;
        case 0x3E:
            a = fetch8();
            return 8;

        case 0x01:
            setBC(fetch16());
            return 12;
        case 0x11:
            setDE(fetch16());
            return 12;
        case 0x21:
            setHL(fetch16());
            return 12;
        case 0x31:
            sp = fetch16();
            return 12;

        case 0x40:
            return 4;
        case 0x41:
            b = c;
            return 4;
        case 0x42:
            b = d;
            return 4;
        case 0x43:
            b = e;
            return 4;
        case 0x44:
            b = h;
            return 4;
        case 0x45:
            b = l;
            return 4;
        case 0x46:
            b = bus.read(getHL());
            return 8;
        case 0x47:
            b = a;
            return 4;

        case 0x48:
            c = b;
            return 4;
        case 0x49:
            return 4;
        case 0x4A:
            c = d;
            return 4;
        case 0x4B:
            c = e;
            return 4;
        case 0x4C:
            c = h;
            return 4;
        case 0x4D:
            c = l;
            return 4;
        case 0x4E:
            c = bus.read(getHL());
            return 8;
        case 0x4F:
            c = a;
            return 4;

        case 0x50:
            d = b;
            return 4;
        case 0x51:
            d = c;
            return 4;
        case 0x52:
            return 4;
        case 0x53:
            d = e;
            return 4;
        case 0x54:
            d = h;
            return 4;
        case 0x55:
            d = l;
            return 4;
        case 0x56:
            d = bus.read(getHL());
            return 8;
        case 0x57:
            d = a;
            return 4;
        
        case 0x58:
            e = b;
            return 4;
        case 0x59:
            e = c;
            return 4;
        case 0x5A:
            e = d;
            return 4;
        case 0x5B:
            return 4;
        case 0x5C:
            e = h;
            return 4;
        case 0x5D:
            e = l;
            return 4;
        case 0x5E:
            e = bus.read(getHL());
            return 8;
        case 0x5F:
            e = a;
            return 4;

        case 0x60:
            h = b;
            return 4;
        case 0x61:
            h = c;
            return 4;
        case 0x62:
            h = d;
            return 4;
        case 0x63:
            h = e;
            return 4;
        case 0x64:
            return 4;
        case 0x65:
            h = l;
            return 4;
        case 0x66:
            h = bus.read(getHL());
            return 8;
        case 0x67:
            h = a;
            return 4;
        
        case 0x68:
            l = b;
            return 4;
        case 0x69:
            l = c;
            return 4;
        case 0x6A:
            l = d;
            return 4;
        case 0x6B:
            l = e;
            return 4;
        case 0x6C:
            l = h;
            return 4;
        case 0x6D:
            return 4;
        case 0x6E:
            l = bus.read(getHL());
            return 8;
        case 0x6F:
            l = a;
            return 4;

        case 0x70:
            bus.write(getHL(), b);
            return 8;
        case 0x71:
            bus.write(getHL(), c);
            return 8;
        case 0x72:
            bus.write(getHL(), d);
            return 8;
        case 0x73:
            bus.write(getHL(), e);
            return 8;
        case 0x74:
            bus.write(getHL(), h);
            return 8;
        case 0x75:
            bus.write(getHL(), l);
            return 8;
        // Not yet implemented HALT
        case 0x77:
            bus.write(getHL(), a);
            return 8;
        
        case 0x78:
            a = b;
            return 4;
        case 0x79:
            a = c;
            return 4;
        case 0x7A:
            a = d;
            return 4;
        case 0x7B:
            a = e;
            return 4;
        case 0x7C:
            a = h;
            return 4;
        case 0x7D:
            a = l;
            return 4;
        case 0x7E:
            a = bus.read(getHL());
            return 8;
        case 0x7F:
            return 4;

        case 0x04:
            increment8(b);
            return 4;
        case 0x05:
            decrement8(b);
            return 4;
        case 0x0C:
            increment8(c);
            return 4;
        case 0x0D:
            decrement8(c);
            return 4;
        case 0x14:
            increment8(d);
            return 4;
        case 0x15:
            decrement8(d);
            return 4;
        case 0x1C:
            increment8(e);
            return 4;
        case 0x1D:
            decrement8(e);
            return 4;
        case 0x24:
            increment8(h);
            return 4;
        case 0x25:
            decrement8(h);
            return 4;
        case 0x2C:
            increment8(l);
            return 4;
        case 0x2D:
            decrement8(l);
            return 4;
        case 0x34: {
            uint8_t value = bus.read(getHL());
            increment8(value);
            bus.write(getHL(), value);
            return 12;
        }
        case 0x35: {
            uint8_t value = bus.read(getHL());
            decrement8(value);
            bus.write(getHL(), value);
            return 12;
        }
        case 0x3C:
            increment8(a);
            return 4;
        case 0x3D:
            decrement8(a);
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