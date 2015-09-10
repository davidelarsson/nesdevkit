#ifndef CPU_INSTRUCTIONS_H
#define CPU_INSTRUCTIONS_H

#include "nes.h"

void cpu_opcodes_init();

/* Array of function pointers to instructions */
void (*cpu_opcodes[256])(nes_t *nes, word address);

/* Help function to push value on stack */
void nes_push_stack(nes_t *nes, byte val);

/* Help function to pull value from stack */
byte nes_pull_stack(nes_t *nes);

/* Instructions */
void cpu_undocumented(nes_t *nes, word address);
void cpu_adc(nes_t *nes, word address);
void cpu_and(nes_t *nes, word address);
void cpu_asl(nes_t *nes, word address);
void cpu_asla(nes_t *nes, word address);
void cpu_bcc(nes_t *nes, word address);
void cpu_bcs(nes_t *nes, word address);
void cpu_beq(nes_t *nes, word address);
void cpu_beq(nes_t *nes, word address);
void cpu_bit(nes_t *nes, word address);
void cpu_bmi(nes_t *nes, word address);
void cpu_bne(nes_t *nes, word address);
void cpu_bpl(nes_t *nes, word address);
void cpu_brk(nes_t *nes, word address);
void cpu_bvc(nes_t *nes, word address);
void cpu_bvs(nes_t *nes, word address);
void cpu_clc(nes_t *nes, word address);
void cpu_cld(nes_t *nes, word address);
void cpu_cli(nes_t *nes, word address);
void cpu_clv(nes_t *nes, word address);
void cpu_cmp(nes_t *nes, word address);
void cpu_cpx(nes_t *nes, word address);
void cpu_cpy(nes_t *nes, word address);
void cpu_dec(nes_t *nes, word address);
void cpu_dex(nes_t *nes, word address);
void cpu_dey(nes_t *nes, word address);
void cpu_eor(nes_t *nes, word address);
void cpu_inc(nes_t *nes, word address);
void cpu_inx(nes_t *nes, word address);
void cpu_iny(nes_t *nes, word address);
void cpu_jmp(nes_t *nes, word address);
void cpu_jsr(nes_t *nes, word address);
void cpu_jsr(nes_t *nes, word address);
void cpu_lda(nes_t *nes, word address);
void cpu_ldx(nes_t *nes, word address);
void cpu_ldy(nes_t *nes, word address);
void cpu_lsra(nes_t *nes, word address);
void cpu_lsr(nes_t *nes, word address);
void cpu_nop(nes_t *nes, word dummy);
void cpu_ora(nes_t *nes, word address);
void cpu_pha(nes_t *nes, word address);
void cpu_php(nes_t *nes, word address);
void cpu_pla(nes_t *nes, word address);
void cpu_plp(nes_t *nes, word address);
void cpu_rti(nes_t *nes, word address);
void cpu_rts(nes_t *nes, word address);
void cpu_rola(nes_t *nes, word address);
void cpu_rol(nes_t *nes, word address);
void cpu_rora(nes_t *nes, word address);
void cpu_ror(nes_t *nes, word address);
void cpu_sbc(nes_t *nes, word address);
void cpu_sec(nes_t *nes, word address);
void cpu_sed(nes_t *nes, word address);
void cpu_sei(nes_t *nes, word address);
void cpu_sta(nes_t *nes, word address);
void cpu_stx(nes_t *nes, word address);
void cpu_sty(nes_t *nes, word address);
void cpu_tax(nes_t *nes, word address);
void cpu_tay(nes_t *nes, word address);
void cpu_tsx(nes_t *nes, word address);
void cpu_txa(nes_t *nes, word address);
void cpu_txs(nes_t *nes, word address);
void cpu_tya(nes_t *nes, word address);

#endif /* CPU_INSTRUCTIONS_H */
