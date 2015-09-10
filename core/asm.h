#ifndef ASM_H
#define ASM_H

#include "defines.h"

/**
 * Assembles a string into a 1-3 byte 6502 instruction 
 *
 * @param bytes 	An array of at least 3 bytes to be assembled
 * @param out		A string of at least 12 chars to be filled by the instruction text
 *
 * @return 			Number of bytes actually used in the assemly.
 */
int nes_asm(char *str, word pc, byte *out);

/**
 * Help function for nes_asm() that identifies the addressing mode of an instruction
 * @param str		String to analyze
 *
 * @return			Mode
 */
addressing_mode_t parse_addressing_mode(char *str, byte *lower, byte *upper);

/**
 * Help function for nes_asm() that identifies the instruction in a string
 * @param str		String to analyze
 *
 * @return			Mode
 */
instruction_t parse_instruction(char *str);

/**
 * Help function for nes_asm() that returns the immediate byte of the argument
 * @param str		String to be analyzed
 *
 * @return			byte argument or -1 on syntax error
 */
int get_imm_byte(char *str);

/**
 * et.c.
 */ 
int get_zp_byte(char *str);
int get_zpx_byte(char *str);
int get_zpy_byte(char *str);
int get_abs_byte_hi(char *str);
int get_abs_byte_lo(char *str);
int get_ind_byte_hi(char *str);
int get_ind_byte_lo(char *str);
int get_ind_byte(char *str);
int get_absx_byte_hi(char *str);
int get_absx_byte_lo(char *str);
int get_absy_byte_hi(char *str);
int get_absy_byte_lo(char *str);
int get_indexx_byte(char *str);
int get_indexy_byte(char *str);
int get_rel_byte(char *str);

#endif /* ASM_H */
