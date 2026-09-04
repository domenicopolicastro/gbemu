/*
 * Experiment/playground used to reason about and understand the DAA instruction.
 *
 * Initial idea:
 * split A into two nibbles and manually normalize values > 9 (A, B, C... F).
 *
 * This is not sufficient for a real DAA implementation because
 * the current value of A alone does not have all the informations 
 * to reconstruct the correct BCD value.
 * 
 * Counterexample is 0x09 + 0x07 = 0x10, but we need 0x16
 * 
 * DAA also needs the N, H and C flags from the previous arithmetic
 * instruction.
 *
 * Keeping this anyway as an artifact of the reasoning process.
 */

#include <iostream>
#include <cstdint>
using namespace std;

int main() {
    uint8_t carry = 0;
    uint8_t a = 0x5C; 
    //0110 1010 

    uint8_t lowerA = a & 0b00001111;
    uint8_t higherA = ((a & 0b11110000) >> 4);
    if (lowerA > 9) { // 0101 -> 0000 > Carry 1
                      // 0110 -> 0110 > Carry 
        carry = 1;
        lowerA -= 10;
    }
    printf("Value of higher A: %2X\n", higherA);
    higherA += carry;

    a = (higherA << 4) + lowerA;

    printf("Value of lower A: %2X\n", lowerA);
    printf("Value of higher A: %2X\n", higherA);
    printf("Value of carry: %d\n", carry);
    printf("Value of A register: %2X\n", a);
    return 0;
}