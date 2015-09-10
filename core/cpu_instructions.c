#include "cpu_instructions.h"
#include "nes.h"
#include "cpu.h"
#include "cpu_mem.h"
#include <stdio.h>

void cpu_opcodes_init()
{
	for(int i = 0; i < 256; i++)
		cpu_opcodes[i] = cpu_undocumented;
	
	cpu_opcodes[0x00] = cpu_brk; /* BRK			*/
	cpu_opcodes[0x01] = cpu_ora; /* ORA ($xx,x)	*/
	cpu_opcodes[0x05] = cpu_ora; /* ORA $xx		*/
	cpu_opcodes[0x06] = cpu_asl; /* ASL $xx		*/
	cpu_opcodes[0x08] = cpu_php; /* PHP			*/
	cpu_opcodes[0x09] = cpu_ora; /* ORA #xx		*/
	cpu_opcodes[0x0A] = cpu_asla;/* ASL A		*/
	cpu_opcodes[0x0D] = cpu_ora; /* ORA $xxxx	*/
	cpu_opcodes[0x0E] = cpu_asl; /* ASL $xxxx	*/
	cpu_opcodes[0x10] = cpu_bpl; /* BPL $xx		*/
	cpu_opcodes[0x11] = cpu_ora; /* ORA ($xx),y	*/
	cpu_opcodes[0x15] = cpu_ora; /* ORA $xx,x	*/
	cpu_opcodes[0x16] = cpu_asl; /* ASL $xx,x	*/
	cpu_opcodes[0x18] = cpu_clc; /* CLC			*/
	cpu_opcodes[0x19] = cpu_ora; /* ORA $xxxx,y	*/
	cpu_opcodes[0x1D] = cpu_ora; /* ORA $xxxx,x	*/
	cpu_opcodes[0x1E] = cpu_asl; /* ASL $xxxx,x	*/
	cpu_opcodes[0x20] = cpu_jsr; /* JSR $xxxx	*/
	cpu_opcodes[0x21] = cpu_and; /* AND ($xx,x)	*/
	cpu_opcodes[0x24] = cpu_bit; /* BIT $xx		*/
	cpu_opcodes[0x25] = cpu_and; /* AND $xx		*/
	cpu_opcodes[0x26] = cpu_rol; /* ROL $xx		*/
	cpu_opcodes[0x28] = cpu_plp; /* PLP			*/
	cpu_opcodes[0x29] = cpu_and; /* AND #xx		*/
	cpu_opcodes[0x2A] = cpu_rola;/* ROL A		*/
	cpu_opcodes[0x2C] = cpu_bit; /* BIT $xxxx	*/
	cpu_opcodes[0x2D] = cpu_and; /* AND $xxxx	*/
	cpu_opcodes[0x2E] = cpu_rol; /* ROL $xxxx	*/
	cpu_opcodes[0x30] = cpu_bmi; /* BMI $xx		*/
	cpu_opcodes[0x31] = cpu_and; /* AND ($xx),y	*/
	cpu_opcodes[0x35] = cpu_and; /* AND $xx,x	*/
	cpu_opcodes[0x36] = cpu_rol; /* ROL $xx,x	*/
	cpu_opcodes[0x38] = cpu_sec; /* SEC			*/
	cpu_opcodes[0x39] = cpu_and; /* AND $xxxx,y	*/
	cpu_opcodes[0x3D] = cpu_and; /* AND $xxxx,x	*/
	cpu_opcodes[0x3E] = cpu_rol; /* ROL $xxxx,x	*/
	cpu_opcodes[0x40] = cpu_rti; /* RTI			*/
	cpu_opcodes[0x41] = cpu_eor; /* EOR ($xx,x)	*/
	cpu_opcodes[0x45] = cpu_eor; /* EOR $xx		*/
	cpu_opcodes[0x46] = cpu_lsr; /* LSR $xx		*/
	cpu_opcodes[0x48] = cpu_pha; /* PHA			*/
	cpu_opcodes[0x49] = cpu_eor; /* EOR #xx		*/
	cpu_opcodes[0x4A] = cpu_lsra;/* LSR A		*/
	cpu_opcodes[0x4C] = cpu_jmp; /* JMP $xxxx	*/
	cpu_opcodes[0x4D] = cpu_eor; /* EOR $xxxx	*/
	cpu_opcodes[0x4E] = cpu_lsr; /* LSR $xxxx	*/
	cpu_opcodes[0x50] = cpu_bvc; /* BVC $xx		*/
	cpu_opcodes[0x51] = cpu_eor; /* EOR ($xx),y	*/
	cpu_opcodes[0x55] = cpu_eor; /* EOR $xx,x	*/
	cpu_opcodes[0x56] = cpu_lsr; /* LSR $xx,x	*/
	cpu_opcodes[0x58] = cpu_cli; /* CLI			*/
	cpu_opcodes[0x59] = cpu_eor; /* EOR $xxxx,y	*/
	cpu_opcodes[0x5D] = cpu_eor; /* EOR $xxxx,x	*/
	cpu_opcodes[0x5E] = cpu_lsr; /* LSR $xxxx,x	*/
	cpu_opcodes[0x60] = cpu_rts; /* RTS			*/
	cpu_opcodes[0x61] = cpu_adc; /* ADC ($xx,x)	*/
	cpu_opcodes[0x65] = cpu_adc; /* ADC $xx		*/
	cpu_opcodes[0x66] = cpu_ror ;/* ROR $xx		*/
	cpu_opcodes[0x68] = cpu_pla; /* PLA			*/
	cpu_opcodes[0x69] = cpu_adc; /* ADC #xx		*/
	cpu_opcodes[0x6A] = cpu_rora;/* ROR A		*/
	cpu_opcodes[0x6C] = cpu_jmp; /* JMP ($xxxx)	*/
	cpu_opcodes[0x6D] = cpu_adc; /* ADC $xxxx	*/
	cpu_opcodes[0x6E] = cpu_ror ;/* ROR $xxxx	*/
	cpu_opcodes[0x70] = cpu_bvs; /* BVS $xx		*/
	cpu_opcodes[0x71] = cpu_adc; /* ADC ($xx),y	*/
	cpu_opcodes[0x75] = cpu_adc; /* ADC $xx,x	*/
	cpu_opcodes[0x76] = cpu_ror ;/* ROR $xx,x	*/
	cpu_opcodes[0x78] = cpu_sei; /* SEI			*/
	cpu_opcodes[0x79] = cpu_adc; /* ADC $xxxx,y	*/
	cpu_opcodes[0x7D] = cpu_adc; /* ADC $xxxx,x	*/
	cpu_opcodes[0x7E] = cpu_ror ;/* ROR $xxxx,x	*/
	cpu_opcodes[0x81] = cpu_sta; /* STA ($xx,x) */
	cpu_opcodes[0x84] = cpu_sty; /* STY $xx		*/
	cpu_opcodes[0x85] = cpu_sta; /* STA $xx		*/
	cpu_opcodes[0x86] = cpu_stx; /* STX $xx		*/
	cpu_opcodes[0x88] = cpu_dey; /* DEY			*/
	cpu_opcodes[0x8A] = cpu_txa; /* TXA			*/
	cpu_opcodes[0x8C] = cpu_sty; /* STY $xxxx	*/
	cpu_opcodes[0x8D] = cpu_sta; /* STA $xxxx	*/
	cpu_opcodes[0x8E] = cpu_stx; /* STX $xxxx	*/
	cpu_opcodes[0x90] = cpu_bcc; /* BCC $xx		*/
	cpu_opcodes[0x91] = cpu_sta; /* STA ($xx),y	*/
	cpu_opcodes[0x94] = cpu_sty; /* STY $xx,x	*/
	cpu_opcodes[0x95] = cpu_sta; /* STA $xx,x	*/
	cpu_opcodes[0x96] = cpu_stx; /* STA $xx,y	*/
	cpu_opcodes[0x98] = cpu_tya; /* TYA			*/
	cpu_opcodes[0x99] = cpu_sta; /* STA $xxxx,y	*/
	cpu_opcodes[0x9A] = cpu_txs; /* TXS			*/
	cpu_opcodes[0x9D] = cpu_sta; /* STA $xxxx,x	*/
	cpu_opcodes[0xA0] = cpu_ldy; /* LDY #xx		*/
	cpu_opcodes[0xA1] = cpu_lda; /* LDA ($xx,x)	*/
	cpu_opcodes[0xA2] = cpu_ldx; /* LDX #xx		*/
	cpu_opcodes[0xA4] = cpu_ldy; /* LDY $xx		*/
	cpu_opcodes[0xA5] = cpu_lda; /* LDA $xx		*/
	cpu_opcodes[0xA6] = cpu_ldx; /* LDX $xx		*/
	cpu_opcodes[0xA8] = cpu_tay; /* TAY			*/
	cpu_opcodes[0xA9] = cpu_lda; /* LDA #xx		*/
	cpu_opcodes[0xAA] = cpu_tax; /* TAX			*/
	cpu_opcodes[0xAC] = cpu_ldx; /* LDY $xxxx	*/
	cpu_opcodes[0xAD] = cpu_lda; /* LDA $xxxx	*/
	cpu_opcodes[0xAE] = cpu_ldx; /* LDX $xxxx	*/
	cpu_opcodes[0xB0] = cpu_bcs; /* BCS $xx		*/
	cpu_opcodes[0xB1] = cpu_lda; /* LDA ($xx),y	*/
	cpu_opcodes[0xB4] = cpu_ldy; /* LDY $xx,x	*/
	cpu_opcodes[0xB5] = cpu_lda; /* LDA $xx,x	*/
	cpu_opcodes[0xB6] = cpu_ldx; /* LDX $xx,y	*/
	cpu_opcodes[0xB8] = cpu_clv; /* CLV			*/
	cpu_opcodes[0xB9] = cpu_lda; /* LDA $xxxx,y	*/
	cpu_opcodes[0xBA] = cpu_tsx; /* TSX			*/
	cpu_opcodes[0xBC] = cpu_ldy; /* LDY $xxxx,x	*/
	cpu_opcodes[0xBD] = cpu_lda; /* LDA $xxxx,x	*/
	cpu_opcodes[0xBE] = cpu_ldx; /* LDX $xxxx,y	*/
	cpu_opcodes[0xC0] = cpu_cpy; /* CPY #xx		*/
	cpu_opcodes[0xC1] = cpu_cmp; /* CMP ($xx,x)	*/
	cpu_opcodes[0xC4] = cpu_cpy; /* CPY $xx		*/
	cpu_opcodes[0xC5] = cpu_cmp; /* CMP $xx		*/
	cpu_opcodes[0xC6] = cpu_dec; /* DEC $xx		*/
	cpu_opcodes[0xC8] = cpu_iny; /* INY			*/
	cpu_opcodes[0xC9] = cpu_cmp; /* CMP #xx		*/
	cpu_opcodes[0xCA] = cpu_dex; /* DEX			*/
	cpu_opcodes[0xCC] = cpu_cpy; /* CPY $xxxx	*/
	cpu_opcodes[0xCD] = cpu_cmp; /* CMP $xxxx	*/
	cpu_opcodes[0xCE] = cpu_dec; /* DEC $xxxx	*/
	cpu_opcodes[0xD0] = cpu_bne; /* BNE $xx		*/
	cpu_opcodes[0xD1] = cpu_cmp; /* CMP ($xx),y	*/
	cpu_opcodes[0xD5] = cpu_cmp; /* CMP $xx,x	*/
	cpu_opcodes[0xD6] = cpu_dec; /* DEC $xx,x	*/
	cpu_opcodes[0xD8] = cpu_cld; /* CLD			*/
	cpu_opcodes[0xD9] = cpu_cmp; /* CMP $xxxx,y	*/
	cpu_opcodes[0xDD] = cpu_cmp; /* CMP $xxxx,x	*/
	cpu_opcodes[0xDE] = cpu_dec; /* DEC $xxxx,x	*/
	cpu_opcodes[0xE0] = cpu_cpx; /* CPX #xx		*/
	cpu_opcodes[0xE1] = cpu_sbc; /* SBC ($xx,x)	*/
	cpu_opcodes[0xE4] = cpu_cpx; /* CPX $xx		*/
	cpu_opcodes[0xE5] = cpu_sbc; /* SBC $xx		*/
	cpu_opcodes[0xE6] = cpu_inc; /* INC $xx		*/
	cpu_opcodes[0xE8] = cpu_inx; /* INX			*/
	cpu_opcodes[0xE9] = cpu_sbc; /* SBC #xx		*/
	cpu_opcodes[0xEA] = cpu_nop; /* NOP			*/
	cpu_opcodes[0xEC] = cpu_cpx; /* CPX $xxxx	*/
	cpu_opcodes[0xED] = cpu_sbc; /* SBC $xxxx	*/
	cpu_opcodes[0xEE] = cpu_inc; /* INC $xxxx	*/
	cpu_opcodes[0xF0] = cpu_beq; /* BEQ $xx		*/
	cpu_opcodes[0xF1] = cpu_sbc; /* SBC ($xx),y	*/
	cpu_opcodes[0xF5] = cpu_sbc; /* SBC $xx,x	*/
	cpu_opcodes[0xF6] = cpu_inc; /* INC $xx,x	*/
	cpu_opcodes[0xF8] = cpu_sed; /* SED			*/
	cpu_opcodes[0xF9] = cpu_sbc; /* SBC $xxxx,y	*/
	cpu_opcodes[0xFD] = cpu_sbc; /* SBC $xxxx,x	*/
	cpu_opcodes[0xFE] = cpu_inc; /* INC $xxxx,x	*/
}

void nes_push_stack(nes_t *nes, byte val)
{
//	printf("Pushing value on stack...\n");
	byte sp = cpu_get_sp(nes_get_cpu(nes));
	nes_cpu_mem_write(nes, STACK_OFFSET + sp, val);
//	printf("Pushing %02X to %04X\n", val, STACK_OFFSET + sp);
	sp--;
	cpu_set_sp(nes_get_cpu(nes), sp);
}

byte nes_pull_stack(nes_t *nes)
{
//	printf("Pulling value from stack...\n");
	byte sp = cpu_get_sp(nes_get_cpu(nes));
	sp++;
	byte val = nes_cpu_mem_read(nes, STACK_OFFSET + sp);
//	printf("Pulling %02X from %04X\n", val, STACK_OFFSET + sp);
	cpu_set_sp(nes_get_cpu(nes), sp);
	return val;
}

void cpu_undocumented(nes_t *nes, word dummy)
{
	printf("UNDOCUMENTED OPCODE!\n");
	(void) nes;
	(void) dummy;
}

void cpu_adc(nes_t *nes, word address)
{
//	printf("ADC\n");
	/* Add byte from CPU memory to accumulator */
	word val = nes_cpu_mem_read(nes, address);
	word res = val + cpu_get_a(nes_get_cpu(nes)) + (cpu_get_c(nes_get_cpu(nes)) ? 1 : 0);

	/* Carry set if result didn't fit in accumulator */
	if(res & 0xFF00)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Set zero flag if result is zero */
	if((res & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(res & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

	res &= 0x00FF;

	/* Overflow... Code stolen from my Java emulator. */
	if (((cpu_get_a(nes_get_cpu(nes)) & 0x80) == 0) && 
			((val & 0x80) == 0) &&
			((res & 0x80) != 0))
		cpu_set_v(nes_get_cpu(nes));
	else if (((cpu_get_a(nes_get_cpu(nes)) & 0x80) != 0) && ((val & 0x80) != 0)
                && ((res & 0x80) == 0))
		cpu_set_v(nes_get_cpu(nes));
	else
		cpu_clear_v(nes_get_cpu(nes));

	/* Return result to accumulator */
	cpu_set_a(nes_get_cpu(nes), res);
}

void cpu_and(nes_t *nes, word address)
{
//	printf("AND\n");

	/* Get byte from CPU memory */
	word val = nes_cpu_mem_read(nes, address);

	/* AND the value with the accumulator */
	byte res = val & cpu_get_a(nes_get_cpu(nes));

	/* Store back to the accumulator */
	cpu_set_a(nes_get_cpu(nes), res);

	/* Set zero flag if result is zero */
	if((res & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(res & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_asla(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("ASL A\n");

	/* Get value from accumulator */
	byte val = cpu_get_a(nes_get_cpu(nes));

	/* If the MSB is set, save it into the carry flag */
	if(val & 0x80)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift left one bit */
	val <<= 1;

	/* Set zero flag if result is zero */
	if(val == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

	/* Store back to accumulator */
	cpu_set_a(nes_get_cpu(nes), val);
}

void cpu_asl(nes_t *nes, word address)
{
//	printf("ASL\n");

	/* Get value from memory */
	byte val = nes_cpu_mem_read(nes, address);

	/* If the MSB is set, save it into the carry flag */
	if(val & 0x80)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift left one bit */
	val <<= 1;

	/* Set zero flag if result is zero */
	if(val == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

	/* Store back to memory */
	nes_cpu_mem_write(nes, address, val);
}

void cpu_bcc(nes_t *nes, word address)
{
//	printf("BCC\n");

	/* In case carry is not set, do nothing */
	if(cpu_get_c(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_bcs(nes_t *nes, word address)
{
//	printf("BCS\n");
	/* In case carry is set, do nothing */
	if(!cpu_get_c(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_beq(nes_t *nes, word address)
{
//	printf("BEQ\n");
	/* In case zero flag is not set, do nothing */
	if(!cpu_get_z(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_bit(nes_t *nes, word address)
{
//	printf("BIT\n");

	/* Get byte from CPU memory */
	byte val = nes_cpu_mem_read(nes, address);

	/* Put bit 6 of the memory value into overflow flag */
	if(val & 0x40)
		cpu_set_v(nes_get_cpu(nes));
	else
		cpu_clear_v(nes_get_cpu(nes));

	/* Set negative flag according to memory value */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

	/* AND the memory value with the accumulator */
	byte res = val & cpu_get_a(nes_get_cpu(nes));

	/* Set zero flag according to result */
	if(res)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));
}

void cpu_bmi(nes_t *nes, word address)
{
//	printf("BMI\n");
	/* In case negative flag is not set, do nothing */
	if(!cpu_get_n(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_bne(nes_t *nes, word address)
{
//	printf("BNE\n");
	/* In case zero flag is set, do nothing */
	if(cpu_get_z(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_bpl(nes_t *nes, word address)
{
//	printf("BPL\n");
	/* In case negative flag is set, do nothing */
	if(cpu_get_n(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_brk(nes_t *nes, word dummy)
{
//	printf("BRK\n");
	(void) dummy;

	/* Push PC + 1 onto stack, MSB first */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	pc++;
	nes_push_stack(nes, pc >> 8);
	nes_push_stack(nes, pc);

	/* State that we are in BRK mode and push flags onto the stack as well */
	cpu_set_b(nes_get_cpu(nes));
	nes_push_stack(nes, cpu_get_p(nes_get_cpu(nes)));

	/* Set interrupt flag */
	cpu_set_i(nes_get_cpu(nes));

	/* Get the NMI vector */
	word nmi_vector = nes_cpu_mem_read(nes, NMI_VECTOR);
	nmi_vector += (nes_cpu_mem_read(nes, NMI_VECTOR + 1) << 8);

	/* Update PC */
	cpu_set_pc(nes_get_cpu(nes), nmi_vector);
}

void cpu_bvc(nes_t *nes, word address)
{
//	printf("BVC\n");
	/* In case overflow flag is set, do nothing */
	if(cpu_get_v(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_bvs(nes_t *nes, word address)
{
//	printf("BVS\n");
	/* In case overflow flag is not set, do nothing */
	if(!cpu_get_v(nes_get_cpu(nes)))
	{
		cpu_set_pc(nes_get_cpu(nes), cpu_get_pc(nes_get_cpu(nes)) + 2);
		return;
	}

	/* The address is a (signed) relative offset to be added to the PC */
	word pc = cpu_get_pc(nes_get_cpu(nes));
//	signed char offset = pc + nes_cpu_mem_read(nes, address) + 2;
	signed char offset = nes_cpu_mem_read(nes, address) + 2;
	cpu_set_pc(nes_get_cpu(nes), offset + pc);

	/* Add clock cycle */
	cpu_add_cycles(nes_get_cpu(nes), 1);

	/* Add one more clock cycle if page boundary is crossed */
	if((pc & 0xFF00) != ((offset + pc) & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);
}

void cpu_clc(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("CLC\n");
	cpu_clear_c(nes_get_cpu(nes));
}

void cpu_cld(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("CLD\n");
	cpu_clear_d(nes_get_cpu(nes));
}

void cpu_cli(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("CLI\n");
	cpu_clear_i(nes_get_cpu(nes));
}

void cpu_clv(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("CLV\n");
	cpu_clear_v(nes_get_cpu(nes));
}

void cpu_cmp(nes_t *nes, word address)
{
//	printf("CMP\n");

	/* Get bytes from CPU memory and accumulator */
	byte val = nes_cpu_mem_read(nes, address);
	byte acc = cpu_get_a(nes_get_cpu(nes));
//	printf("Comparing A: %02X with memory: %02X\n", acc, val);

	/* Handle carry flag */
	if(acc >= val)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));

	/* Make comparsion */
	byte res = acc - val;
	
	/* Handle zero flag. Set if accumulator is equal to memory value */
	if(res)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));
	
	/* Handle negative flag */
	if(res & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_cpx(nes_t *nes, word address)
{
//	printf("CPX\n");

	/* Get bytes from CPU memory and the x register */
	byte val = nes_cpu_mem_read(nes, address);
	byte x = cpu_get_x(nes_get_cpu(nes));
//	printf("Comparing X: %02X with memory: %02X\n", x, val);

	/* Handle carry flag */
	if(x >= val)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));

	/* Make comparsion */
	byte res = x - val;
	
	/* Handle zero flag. Set if accumulator is equal to memory value */
	if(res)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));
	
	/* Handle negative flag */
	if(res & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_cpy(nes_t *nes, word address)
{
//	printf("CPY\n");

	/* Get bytes from CPU memory and the y register */
	byte val = nes_cpu_mem_read(nes, address);
	byte y = cpu_get_y(nes_get_cpu(nes));
//	printf("Comparing Y: %02X with memory: %02X\n", y, val);

	/* Handle carry flag */
	if(y >= val)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));

	/* Make comparsion */
	byte res = y - val;
	
	/* Handle zero flag. Set if accumulator is equal to memory value */
	if(res)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));
	
	/* Handle negative flag */
	if(res & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_dec(nes_t *nes, word address)
{
//	printf("DEC\n");

	/* Read memory value, decrement, write back */
	byte val = nes_cpu_mem_read(nes, address);
	val--;
	nes_cpu_mem_write(nes, address, val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_dex(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("DEX\n");

	/* Read value from X register, decrement, write back */
	byte val = cpu_get_x(nes_get_cpu(nes));
	val--;
	cpu_set_x(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_dey(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("DEY\n");

	/* Read value from Y register, decrement, write back */
	byte val = cpu_get_y(nes_get_cpu(nes));
	val--;
	cpu_set_y(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_eor(nes_t *nes, word address)
{
//	printf("EOR\n");

	/* Get byte from CPU memory */
	word val = nes_cpu_mem_read(nes, address);

	/* XOR the value with the accumulator */
	byte res = val ^ cpu_get_a(nes_get_cpu(nes));

	/* Store back to the accumulator */
	cpu_set_a(nes_get_cpu(nes), res);

	/* Set zero flag if result is zero */
	if((res & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(res & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_inc(nes_t *nes, word address)
{
//	printf("INC\n");

	/* Read memory value, increment, write back */
	byte val = nes_cpu_mem_read(nes, address);
	val++;
	nes_cpu_mem_write(nes, address, val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_inx(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("DEX\n");

	/* Read value from X register, increment, write back */
	byte val = cpu_get_x(nes_get_cpu(nes));
	val++;
	cpu_set_x(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_iny(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("DEY\n");

	/* Read value from Y register, increment, write back */
	byte val = cpu_get_y(nes_get_cpu(nes));
	val++;
	cpu_set_y(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_jmp(nes_t *nes, word address)
{
//	printf("JMP\n");

	/* Just change the PC */
	cpu_set_pc(nes_get_cpu(nes), address);
}

void cpu_jsr(nes_t *nes, word address)
{
//	printf("JSR\n");

	/* Push PC + 2 (i.e., return address minus one) onto stack */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	pc += 2;

	/* High byte first, then low */
	nes_push_stack(nes, (byte) (pc >> 8));
	nes_push_stack(nes, (byte) (pc & 0x00FF));

	/* Jump to address */
	cpu_set_pc(nes_get_cpu(nes), address);
}

void cpu_lda(nes_t *nes, word address)
{
//	printf("LDA\n");
	/* Get byte from CPU memory */
	byte val = nes_cpu_mem_read(nes, address);
	cpu_set_a(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

}

void cpu_ldx(nes_t *nes, word address)
{
//	printf("LDX\n");

	/* Put the value in CPU mem that the address points to into the X register */
	byte val = nes_cpu_mem_read(nes, address);
	cpu_set_x(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

}

void cpu_ldy(nes_t *nes, word address)
{
//	printf("LDY\n");

	/* Put the value in CPU mem that the address points to into the Y register */
	byte val = nes_cpu_mem_read(nes, address);
	cpu_set_y(nes_get_cpu(nes), val);

	/* Handle zero flag */
	if(val)
		cpu_clear_z(nes_get_cpu(nes));
	else
		cpu_set_z(nes_get_cpu(nes));

	/* Handle negative flag */
	if(val & MASK_NEGATIVE)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

}

void cpu_lsra(nes_t *nes, word dummy)
{
//	printf("LSRA\n");

	(void) dummy;

	/* Get byte from CPU accumulator */
	word val = cpu_get_a(nes_get_cpu(nes));

	/* Save bit 0 in the carry flag */
	if(val & 0x01)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the right */
	val >>= 1;

	/* Store back to the accumulator */
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* The result is never negative */
	cpu_clear_n(nes_get_cpu(nes));
}

void cpu_lsr(nes_t *nes, word address)
{
//	printf("LSR\n");

	/* Get byte from CPU memory */
	word val = nes_cpu_mem_read(nes, address);

	/* Save bit 0 in the carry flag */
	if(val & 0x01)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the right */
	val >>= 1;

	/* Store back to the cpu memory */
	nes_cpu_mem_write(nes, address, val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* The result is never negative */
	cpu_clear_n(nes_get_cpu(nes));
}

void cpu_ora(nes_t *nes, word address)
{
//	printf("ORA\n");

	/* Get byte from CPU memory */
	word val = nes_cpu_mem_read(nes, address);

	/* OR the value with the accumulator */
	byte res = val | cpu_get_a(nes_get_cpu(nes));

	/* Store back to the accumulator */
	cpu_set_a(nes_get_cpu(nes), res);

	/* Set zero flag if result is zero */
	if((res & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(res & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_rola(nes_t *nes, word dummy)
{
//	printf("ROL A\n");

	(void) dummy;

	/* Get byte from the accumulator, and carry flag */
	word val = cpu_get_a(nes_get_cpu(nes));
	bool old_carry = cpu_get_c(nes_get_cpu(nes));

	/* Save bit 7 in the carry flag */
	if(val & 0x80)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the left */
	val <<= 1;

	/* Old carry is put into bit 0 */
	if(old_carry)
		val |= 0x01;

	/* Store back to the accumulator */
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_rol(nes_t *nes, word address)
{
//	printf("ROL\n");

	/* Get byte from cpu memory, and carry flag */
	word val = nes_cpu_mem_read(nes, address);
	bool old_carry = cpu_get_c(nes_get_cpu(nes));

	/* Save bit 7 in the carry flag */
	if(val & 0x80)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the left */
	val <<= 1;

	/* Old carry is put into bit 0 */
	if(old_carry)
		val |= 0x01;

	/* Store back to CPU memory */
	nes_cpu_mem_write(nes, address, val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_rora(nes_t *nes, word dummy)
{
//	printf("ROR A\n");

	(void) dummy;

	/* Get byte from accumulator, and carry flag */
	word val = cpu_get_a(nes_get_cpu(nes));
	bool old_carry = cpu_get_c(nes_get_cpu(nes));

	/* Save bit 0 in the carry flag */
	if(val & 0x01)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the right */
	val >>= 1;

	/* Old carry is put into bit 7 */
	if(old_carry)
		val |= 0x80;

	/* Store back to accumulator */
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_ror(nes_t *nes, word address)
{
//	printf("ROR\n");

	/* Get byte from cpu memory, and carry flag */
	word val = nes_cpu_mem_read(nes, address);
	bool old_carry = cpu_get_c(nes_get_cpu(nes));

	/* Save bit 0 in the carry flag */
	if(val & 0x01)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Shift bits one step to the right */
	val >>= 1;

	/* Old carry is put into bit 7 */
	if(old_carry)
		val |= 0x80;

	/* Store back to CPU memory */
	nes_cpu_mem_write(nes, address, val);

	/* Set zero flag if result is zero */
	if((val & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_sbc(nes_t *nes, word address)
{
//	printf("SBC\n");
	/* Subtract CPU memory byte and carry flag from accumulator */
	word val = nes_cpu_mem_read(nes, address);
	word res = cpu_get_a(nes_get_cpu(nes)) - val - (cpu_get_c(nes_get_cpu(nes)) ? 0 : 1);

	/* Carry set if result didn't fit in accumulator */
	if(res < 0x100)
		cpu_set_c(nes_get_cpu(nes));
	else
		cpu_clear_c(nes_get_cpu(nes));
	
	/* Set zero flag if result is zero */
	if((res & 0x00FF) == 0)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(res & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));

	//res &= 0x00FF;

	/* Overflow... I don't get it. Code stolen. */
	if(((cpu_get_a(nes_get_cpu(nes)) ^ res) & 0x80) &&
			((cpu_get_a(nes_get_cpu(nes)) ^ val) & 0x80))
	{
//		printf("Setting V flag\n");
		cpu_set_v(nes_get_cpu(nes));
	}
	else
	{
//		printf("Clearing V flag\n");
		cpu_clear_v(nes_get_cpu(nes));
	}

	/* Return result to accumulator */
	cpu_set_a(nes_get_cpu(nes), res);
}

void cpu_sec(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("SEC\n");
	cpu_set_c(nes_get_cpu(nes));
}

void cpu_sed(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("SED\n");
	cpu_set_d(nes_get_cpu(nes));
}

void cpu_sei(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("SEI\n");
	cpu_set_i(nes_get_cpu(nes));
}

void cpu_sta(nes_t *nes, word address)
{
//	printf("STA\n");
	/* Put value of accumulator into memory */
	byte val = cpu_get_a(nes_get_cpu(nes));
	nes_cpu_mem_write(nes, address, val);

	/* No effect on flags */
}

void cpu_stx(nes_t *nes, word address)
{
//	printf("STX\n");
	/* Put value of the X register into memory */
	byte val = cpu_get_x(nes_get_cpu(nes));
	nes_cpu_mem_write(nes, address, val);

	/* No effect on flags */
}

void cpu_sty(nes_t *nes, word address)
{
//	printf("STY\n");
	/* Put value of the Y register into memory */
	byte val = cpu_get_y(nes_get_cpu(nes));
	nes_cpu_mem_write(nes, address, val);

	/* No effect on flags */
}

void cpu_nop(nes_t *nes, word dummy)
{
//	printf("NOP\n");
	(void) nes;
	(void) dummy;
}

void cpu_pha(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("PHA\n");

	nes_push_stack(nes, cpu_get_a(nes_get_cpu(nes)));
}

void cpu_php(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("PHP\n");

	nes_push_stack(nes, cpu_get_p(nes_get_cpu(nes)));
}

void cpu_pla(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("PLA\n");

	byte val = nes_pull_stack(nes);
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_plp(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("PLP\n");

	cpu_set_p(nes_get_cpu(nes), nes_pull_stack(nes));
}

void cpu_rti(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("RTI\n");

	/* Pull flags from stack */
	word flags = nes_pull_stack(nes);
	cpu_set_p(nes_get_cpu(nes), flags);

	/* Pull PC from stack */
	word pc = nes_pull_stack(nes);
	pc += (nes_pull_stack(nes) << 8);

	/* Jump to address */
	cpu_set_pc(nes_get_cpu(nes), pc);
}

void cpu_rts(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("RTS\n");

	/* Pull PC from stack */
	word pc = nes_pull_stack(nes);
	pc += (nes_pull_stack(nes) << 8);

	/* Add one */
	pc++;

	/* Jump to address */
	cpu_set_pc(nes_get_cpu(nes), pc);
}

void cpu_tax(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TAX\n");

	/* Copy accumulator to the X register */
	byte val = cpu_get_a(nes_get_cpu(nes));
	cpu_set_x(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_tay(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TAY\n");

	/* Copy accumulator to the Y register */
	byte val = cpu_get_a(nes_get_cpu(nes));
	cpu_set_y(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_tsx(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TSX\n");

	/* Copy stack pointer to the X register */
	byte val = cpu_get_sp(nes_get_cpu(nes));
	cpu_set_x(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_txa(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TXA\n");

	/* Copy X register into the accumulator */
	byte val = cpu_get_x(nes_get_cpu(nes));
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

void cpu_txs(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TXS\n");

	/* Copy X register into the stack pointer */
	byte val = cpu_get_x(nes_get_cpu(nes));
	cpu_set_sp(nes_get_cpu(nes), val);

	/* Flags are not affected */
}

void cpu_tya(nes_t *nes, word dummy)
{
	(void) dummy;
//	printf("TYA\n");

	/* Copy Y register into the accumulator */
	byte val = cpu_get_y(nes_get_cpu(nes));
	cpu_set_a(nes_get_cpu(nes), val);

	/* Set zero flag if result is zero */
	if(!val)
		cpu_set_z(nes_get_cpu(nes));
	else
		cpu_clear_z(nes_get_cpu(nes));

	/* Set negative flag if MSB is set */
	if(val & 0x80)
		cpu_set_n(nes_get_cpu(nes));
	else
		cpu_clear_n(nes_get_cpu(nes));
}

