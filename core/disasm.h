#ifndef DISASM_H
#define DISASM_H

#include "defines.h"
/**
 * Disassemble a byte string.
 *
 * @param bytes 	An array of at least 3 bytes to be disassembled
 * @param out		A string of at least 12 char to be filled by the instruction text
 *
 * @return len		Number of bytes actually used in the disassemly.
 */
unsigned nes_disasm(byte *bytes, word pc, char *out);

#endif /* DISASM_H */

