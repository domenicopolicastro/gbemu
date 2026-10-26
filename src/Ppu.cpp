
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