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
    
    /* 
    *  CALL Operations 
    *  0xC4 Call NZ, a16 1100 0100
    *  0xCC CALL Z, a16  1100 1100
    *  0xD4 CALL NC, a16 1101 0100
    *  0xDC CALL C, a16  1101 1100  
    *  
    *  Pattern:          110X X100  
    */
    if ((opcode & 0b11100111) == 0b11000100) {
        uint8_t condition = (opcode >> 3) & 0b11;
        uint16_t address = fetch16();
        bool call;
        switch (condition) {
            case 0: call = !getZeroFlag(); break;
            case 1: call = getZeroFlag(); break;
            case 2: call = !getCarryFlag(); break;
            case 3: call = getCarryFlag(); break;
        }
        if (call) {
            push16(pc);
            pc = address;
            return 24;
        }
        return 12;
    }
    

    /*
    *  RET Operations
    *  0xC0 1100 0000
    *  0xC8 1100 1000
    *  0xD0 1101 0000
    *  0xD8 1101 1000
    *       110X X000
    */
   if ((opcode & 0b11100111) == 0b11000000) {
        uint8_t condition = (opcode >> 3) & 0b11;
        bool takeReturn;
        switch (condition) {
            case 0: takeReturn = !getZeroFlag(); break;
            case 1: takeReturn = getZeroFlag(); break;
            case 2: takeReturn = !getCarryFlag(); break;
            case 3: takeReturn = getCarryFlag(); break;         
        }
        if (takeReturn) {
            pc = pop16();
            return 20;
        }
        return 8;
   }
    

   /*
    *   INDIRECT A STORES
    *   0x02  LD (BC),A     0000 0010
    *   0x12  LD (DE),A     0001 0010
    *   0x22  LD (HL+),A    0010 0010
    *   0x32  LD (HL-),A    0011 0010
    *   Pattern:            00XX 0010
   */
   if ((opcode & 0b11001111) == 0b00000010) {
        uint8_t operation = (opcode >> 4) & 0b11;
        switch(operation) {
            case 0: bus.write(getBC(), a); break;
            case 1: bus.write(getDE(), a); break;
            case 2: bus.write(getHL(), a); setHL(getHL() + 1); break;
            case 3: bus.write(getHL(), a); setHL(getHL() - 1); break;
        }
        return 8;
   }
   /*
    *   INDIRECT A LOADS
    *   0x0A  LD A,(BC)     0000 1010
    *   0x1A  LD A,(DE)     0001 1010
    *   0x2A  LD A,(HL+)    0010 1010
    *   0x3A  LD A,(HL-)    0011 1010
    *   Pattern:            00XX 1010
   */
   if ((opcode & 0b11001111) == 0b00001010) {
       uint8_t operation = (opcode >> 4) & 0b11;
       switch (operation) {
            case 0: a = bus.read(getBC()); break;
            case 1: a = bus.read(getDE()); break;
            case 2: a = bus.read(getHL()); setHL(getHL() + 1); break;
            case 3: a = bus.read(getHL()); setHL(getHL() - 1); break;
       }
       return 8;
   }

   /*
   *    LD (a16), A - 0xEA - 1110 1010
   *    LD A, (a16) - 0xFA - 1111 1010
   *    Pattern 111X 1010
   */
  if ((opcode & 0b11101111) == 0b11101010) {
    bool loadInA = (opcode >> 4) & 0b1;
    if(loadInA)  a = bus.read(fetch16()); 
    else bus.write(fetch16(), a);
    return 16;
  }
  
  if ((opcode & 0b11101111) == 0b11100000) {
    bool loadInA = (opcode >> 4) & 0b1;
    uint16_t address = 0xFF00 + fetch8();
    if(loadInA) a = bus.read(address); 
    else bus.write(address, a);
    return 12;
  }
  // 0xE2 0xF2
  // 1110 0010
  // 1111 0010
  if ((opcode & 0b11101111) == 0b11100010) {
    bool loadInA = (opcode >> 4) & 0b1;
    uint16_t address = 0xFF00 + c;
    if(loadInA) a = bus.read(address); 
    else bus.write(address, a);
    return 8;
  }

  /*
  *     INC r16 instructions
  * INC BC 0x03 0000 0011
  * INC DE 0X13 0001 0011
  * INC HL 0x23 0010 0011
  * INC SP 0x33 0011 0011
  * Pattern is 00XX 0011
  *     DEC r16 instructions
  * DEC BC 0x0B 0000 1011
  * DEC DE 0x1B 0001 1011
  * DEC HL 0x2B 0010 1011
  * DEC SP 0x3B 0011 1011
  * Pattern is 00XX 1011
  * 
  * We can join those two sets by making 00XX S011
  */
    if ((opcode & 0b11000111) == 0b00000011) {
        uint8_t reg = (opcode >> 4) & 0b11;
        int delta = ((opcode >> 3) & 0b1) ? -1 : 1;
        switch (reg) {
            case 0: setBC(getBC() + delta); break;
            case 1: setDE(getDE() + delta); break;
            case 2: setHL(getHL() + delta); break;
            case 3: sp = sp + delta; break;
        }
        return 8;
    }

    if ((opcode & 0b11001111) == 0b00001001) {
        uint8_t reg = (opcode >> 4) & 0b11;
        uint16_t operand;
        uint16_t oldHL = getHL();
        switch (reg) {
            case 0: operand = getBC(); break;
            case 1: operand = getDE(); break;
            case 2: operand = getHL(); break;
            case 3: operand = sp;
        }
        setHL(oldHL + operand);
        bool carry = static_cast<uint32_t>(oldHL) + operand > 0xFFFF;
        bool halfCarry = (oldHL & 0x0FFF) + (operand & 0x0FFF) > 0x0FFF;
        setCarryFlag(carry);
        setHalfCarryFlag(halfCarry);
        setSubtractFlag(false);
        return 8;
    }

    /*
    *
    * C6 ADD A,n8 1100 0110
    * CE ADC A,n8 1100 1110
    * D6 SUB n8   1101 0110
    * DE SBC A,n8 1101 1110
    * 
    * E6 AND n8   1110 0110
    * EE XOR n8   1110 1110
    * F6 OR n8    1111 0110
    * FE CP n8    1111 1110
    *             11XX X110
    */
   if ((opcode & 0b11000111) == 0b11000110) {
        uint8_t op = (opcode >> 3) & 0b111;
        uint8_t value = fetch8();
        switch(op) {
            case 0b000: add8(value); break;
            case 0b001: addWithCarry8(value); break;
            case 0b010: sub8(value); break;
            case 0b011: subWithCarry8(value); break;
            case 0b100: and8(value); break;
            case 0b101: xor8(value); break;
            case 0b110: or8(value); break;
            case 0b111: compare8(value); break;
        }
        return 8;
   }

    switch(opcode) {
        case 0x00:
            return 4;
        
        case 0x08: {
            uint16_t address = fetch16();
            bus.write(address, static_cast<uint8_t>(sp));
            bus.write(address + 1, static_cast<uint8_t>(sp >> 8));
            return 20;
        }
        
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
        // Generic CALL a16
        case 0xCD: {
            uint16_t address = fetch16();
            push16(pc);
            pc = address;
            return 24;
        }

        // Generic RET
        case 0xC9: 
            pc = pop16();
            return 16;

        case 0xE8: {
            int8_t value = static_cast<int8_t>(fetch8());
            uint16_t oldSp = sp;
            uint8_t rawValue = static_cast<uint8_t>(value);
            sp = oldSp + value;
            bool carry = (oldSp & 0x00FF) + rawValue > 0xFF;
            bool halfCarry = (oldSp & 0x0F) + (rawValue & 0x0F) > 0x0F;
            setHalfCarryFlag(halfCarry);
            setCarryFlag(carry);
            setZeroFlag(false);
            setSubtractFlag(false);
            return 16;
        }
        case 0xE9: 
            pc = getHL();
            return 4;
        
        case 0xF8:{
            int8_t value = static_cast<int8_t>(fetch8());
            uint16_t result = sp + value;
            setHL(result);
            bool carry = (sp & 0x00FF) + (static_cast<uint8_t>(value)) > 0x00FF;
            bool halfCarry = (sp & 0x000F) + (static_cast<uint8_t>(value) & 0x0F) > 0x000F;
            setCarryFlag(carry);
            setHalfCarryFlag(halfCarry);
            setZeroFlag(false);
            setSubtractFlag(false);
            return 12;
        }
        case 0xF9:
            sp = getHL();
            return 8;

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