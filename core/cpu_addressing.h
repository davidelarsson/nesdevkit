#ifndef CPU_ADDRESSING_H
#define CPU_ADDRESSING_H
#include "nes.h"

/** Initialize cpu_address_modes array */
void cpu_address_init();

/**
 * Get address of byte in memory by immediate addressing mode
 *
 * @param nes the NES from which the PC of the CPU can be fetched
 * @return address of byte
 */
word cpu_address_imm(nes_t *nes);

/** Get byte from memory by zero page addressing mode */
word cpu_address_zp(nes_t *nes);

/** Get byte from memory by zero page, X addressing mode */
word cpu_address_zpx(nes_t *nes);

/** Get byte from memory by zero page, Y addressing mode */
word cpu_address_zpy(nes_t *nes);

/** Get byte from memory by absolute addressing mode */
word cpu_address_abs(nes_t *nes);

/** Get byte from memory by indirect addressing mode */
word cpu_address_ind(nes_t *nes);

/** Get byte from memory by absolute, X addressing mode */
word cpu_address_absx(nes_t *nes);

/** Get byte from memory by absolute, X addressing mode with clock cycle added due to page boundary crossing */
word cpu_address_absxpb(nes_t *nes);

/** Get byte from memory by absolute, Y addressing mode */
word cpu_address_absy(nes_t *nes);

/** Get byte from memory by absolute, Y addressing mode with clock cycle added due to page boundary crossing */
word cpu_address_absypb(nes_t *nes);

/** Get byte from memory by (indirect,x) addressing mode */
word cpu_address_indexx(nes_t *nes);

/** Get byte from memory by (indirect),y addressing mode */
word cpu_address_indexy(nes_t *nes);

/** Get byte from memory by (indirect),y addressing mode with clock cycle added due to page boundary crossing */
word cpu_address_indexypb(nes_t *nes);

/** Get byte from memory by relative addressing mode */
word cpu_address_rel(nes_t *nes);

/** Don't actually get any byte from memory */
word cpu_address_dummy(nes_t *nes);

/** Array of addressing modes */
word (*cpu_address_modes[256])(nes_t *nes);

#endif /* CPU_ADDRESSING_H */
