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
        case 0x00: // NOP
            return 4;
        case 0x3E: // LD A,d8
            a = fetch8();
            return 8;
        case 0x01: // LD BC,d16
            setBC(fetch16());
            return 12;
        default:
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