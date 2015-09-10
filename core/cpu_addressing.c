#include "cpu_addressing.h"
#include "cpu.h"
#include "cpu_mem.h"
#include <stdio.h>

void cpu_address_init()
{
	for(int i = 0; i < 256; i++)
		cpu_address_modes[i] = cpu_address_dummy;
	
	cpu_address_modes[0x00] = cpu_address_dummy;	/* BRK			*/
	cpu_address_modes[0x01] = cpu_address_indexx;	/* ORA ($xx,x)	*/
	cpu_address_modes[0x05] = cpu_address_zp;		/* ORA $xx		*/
	cpu_address_modes[0x06] = cpu_address_zp;		/* ASL $xx		*/
	cpu_address_modes[0x08] = cpu_address_dummy;	/* PHP			*/
	cpu_address_modes[0x09] = cpu_address_imm;		/* ORA #xx		*/
	cpu_address_modes[0x0A] = cpu_address_dummy;	/* ASL A		*/
	cpu_address_modes[0x0D] = cpu_address_abs;		/* ORA $xxxx	*/
	cpu_address_modes[0x0E] = cpu_address_abs;		/* ASL $xxxx	*/
	cpu_address_modes[0x10] = cpu_address_rel;	 	/* BPL $xx		*/
	cpu_address_modes[0x11] = cpu_address_indexy;	/* ORA ($xx),y	*/
	cpu_address_modes[0x15] = cpu_address_zpx;		/* ORA $xx,x	*/
	cpu_address_modes[0x16] = cpu_address_zpx;		/* ASL $xx,x	*/
	cpu_address_modes[0x18] = cpu_address_dummy;	/* CLC			*/
	cpu_address_modes[0x19] = cpu_address_absy;		/* ORA $xxxx,y	*/
	cpu_address_modes[0x1D] = cpu_address_absx;		/* ORA $xxxx,x	*/
	cpu_address_modes[0x1E] = cpu_address_absx;		/* ASL $xxxx,x	*/
	cpu_address_modes[0x20] = cpu_address_abs;		/* JSR $xxxx	*/
	cpu_address_modes[0x21] = cpu_address_indexx;	/* AND ($xx,x)	*/
	cpu_address_modes[0x24] = cpu_address_zp;		/* BIT $xx		*/
	cpu_address_modes[0x25] = cpu_address_zp;		/* AND $xx		*/
	cpu_address_modes[0x26] = cpu_address_zp;		/* ROL $xx		*/
	cpu_address_modes[0x28] = cpu_address_dummy;	/* PLP			*/
	cpu_address_modes[0x29] = cpu_address_imm;		/* AND #xx		*/
	cpu_address_modes[0x2A] = cpu_address_dummy;	/* ROL A		*/
	cpu_address_modes[0x2C] = cpu_address_abs;		/* BIT $xxxx	*/
	cpu_address_modes[0x2D] = cpu_address_abs;		/* AND $xxxx	*/
	cpu_address_modes[0x2E] = cpu_address_abs;		/* ROL $xxxx	*/
	cpu_address_modes[0x30] = cpu_address_rel;	 	/* BMI $xx		*/
	cpu_address_modes[0x31] = cpu_address_indexypb;	/* AND ($xx),y	*/
	cpu_address_modes[0x35] = cpu_address_zpx;		/* AND $xx,x	*/
	cpu_address_modes[0x36] = cpu_address_zpx;		/* ROL $xx,x	*/
	cpu_address_modes[0x38] = cpu_address_dummy;	/* SEC			*/
	cpu_address_modes[0x39] = cpu_address_absypb;	/* AND $xxxx,y	*/
	cpu_address_modes[0x3D] = cpu_address_absxpb;	/* AND $xxxx,x	*/
	cpu_address_modes[0x3E] = cpu_address_absx;		/* ROL $xxxx,x	*/
	cpu_address_modes[0x40] = cpu_address_dummy;	/* RTI			*/
	cpu_address_modes[0x41] = cpu_address_indexx;	/* EOR ($xx,x)	*/
	cpu_address_modes[0x45] = cpu_address_zp;		/* EOR $xx		*/
	cpu_address_modes[0x46] = cpu_address_zp;		/* LSR $xx		*/
	cpu_address_modes[0x48] = cpu_address_dummy;	/* PHA			*/
	cpu_address_modes[0x49] = cpu_address_imm;		/* EOR #xx		*/
	cpu_address_modes[0x4A] = cpu_address_dummy;	/* LSR A		*/
	cpu_address_modes[0x4C] = cpu_address_abs;		/* JMP $xxxx	*/
	cpu_address_modes[0x4D] = cpu_address_abs;		/* EOR $xxxx	*/
	cpu_address_modes[0x4E] = cpu_address_abs;		/* LSR $xxxx	*/
	cpu_address_modes[0x50] = cpu_address_rel;	 	/* BVC $xx		*/
	cpu_address_modes[0x51] = cpu_address_indexy;	/* EOR ($xx),y	*/
	cpu_address_modes[0x55] = cpu_address_zpx;		/* EOR $xx,x	*/
	cpu_address_modes[0x56] = cpu_address_zpx;		/* LSR $xx,x	*/
	cpu_address_modes[0x58] = cpu_address_dummy;	/* CLI			*/
	cpu_address_modes[0x59] = cpu_address_absy;		/* EOR $xxxx,y	*/
	cpu_address_modes[0x5D] = cpu_address_absx;		/* EOR $xxxx,x	*/
	cpu_address_modes[0x5E] = cpu_address_absx;		/* LSR $xxxx,x	*/
	cpu_address_modes[0x60] = cpu_address_dummy;	/* RTS			*/
	cpu_address_modes[0x61] = cpu_address_indexx;	/* ADC ($xx,x)	*/
	cpu_address_modes[0x65] = cpu_address_zp;		/* ADC $xx		*/
	cpu_address_modes[0x66] = cpu_address_zp;		/* ROR $xx		*/
	cpu_address_modes[0x68] = cpu_address_dummy;	/* PLA			*/
	cpu_address_modes[0x69] = cpu_address_imm;		/* ADC #xx		*/
	cpu_address_modes[0x6A] = cpu_address_dummy;	/* ROR A		*/
	cpu_address_modes[0x6C] = cpu_address_ind;		/* JMP ($xxxx)	*/
	cpu_address_modes[0x6D] = cpu_address_abs;		/* ADC $xxxx	*/
	cpu_address_modes[0x6E] = cpu_address_abs;		/* ROR $xxxx	*/
	cpu_address_modes[0x70] = cpu_address_rel;	 	/* BVS $xx		*/
	cpu_address_modes[0x71] = cpu_address_indexypb;	/* ADC ($xx),y	*/
	cpu_address_modes[0x75] = cpu_address_zpx;		/* ADC $xx,x	*/
	cpu_address_modes[0x76] = cpu_address_zpx;		/* ROR $xx,x	*/
	cpu_address_modes[0x78] = cpu_address_dummy;	/* SEI			*/
	cpu_address_modes[0x79] = cpu_address_absypb;	/* ADC $xxxx,y	*/
	cpu_address_modes[0x7D] = cpu_address_absxpb;	/* ADC $xxxx,x	*/
	cpu_address_modes[0x7E] = cpu_address_absx;		/* ROR $xxxx,x	*/
	cpu_address_modes[0x81] = cpu_address_indexx;	/* STA ($xx,x)	*/
	cpu_address_modes[0x84] = cpu_address_zp;		/* STY $xx		*/
	cpu_address_modes[0x85] = cpu_address_zp;		/* STA $xx		*/
	cpu_address_modes[0x86] = cpu_address_zp; 		/* STX $xx		*/
	cpu_address_modes[0x88] = cpu_address_dummy; 	/* DEY			*/
	cpu_address_modes[0x8A] = cpu_address_dummy;	/* TXA	 		*/
	cpu_address_modes[0x8C] = cpu_address_abs;		/* STY $xxxx	*/
	cpu_address_modes[0x8D] = cpu_address_abs; 		/* STA $xxxx	*/
	cpu_address_modes[0x8E] = cpu_address_abs; 		/* STX $xxxx	*/
	cpu_address_modes[0x90] = cpu_address_rel;	 	/* BCC $xx		*/
	cpu_address_modes[0x91] = cpu_address_indexy; 	/* STA ($xx),y	*/
	cpu_address_modes[0x94] = cpu_address_zpx; 		/* STY $xx,x	*/
	cpu_address_modes[0x95] = cpu_address_zpx; 		/* STA $xx,x	*/
	cpu_address_modes[0x96] = cpu_address_zpy; 		/* STX $xx,y	*/
	cpu_address_modes[0x98] = cpu_address_dummy;	/* TYA	 		*/
	cpu_address_modes[0x99] = cpu_address_absy; 	/* STA $xxxx,y	*/
	cpu_address_modes[0x9A] = cpu_address_dummy;	/* TXS	 		*/
	cpu_address_modes[0x9D] = cpu_address_absx; 	/* STA $xxxx,x	*/
	cpu_address_modes[0xA0] = cpu_address_imm; 		/* LDY #xx		*/
	cpu_address_modes[0xA1] = cpu_address_indexx; 	/* LDA ($xx,x)	*/
	cpu_address_modes[0xA2] = cpu_address_imm; 		/* LDX #xx		*/
	cpu_address_modes[0xA4] = cpu_address_zp; 		/* LDY $xx		*/
	cpu_address_modes[0xA5] = cpu_address_zp; 		/* LDA $xx 		*/
	cpu_address_modes[0xA6] = cpu_address_zp; 		/* LDX $xx		*/
	cpu_address_modes[0xA8] = cpu_address_dummy;	/* TAY	 		*/
	cpu_address_modes[0xA9] = cpu_address_imm; 		/* LDA #xx		*/
	cpu_address_modes[0xAA] = cpu_address_dummy;	/* TAX	 		*/
	cpu_address_modes[0xAC] = cpu_address_abs; 		/* LDY $xxxx	*/
	cpu_address_modes[0xAD] = cpu_address_abs; 		/* LDA $xxxx	*/
	cpu_address_modes[0xAE] = cpu_address_abs; 		/* LDX $xxxx	*/
	cpu_address_modes[0xB0] = cpu_address_rel;	 	/* BCS $xx		*/
	cpu_address_modes[0xB1] = cpu_address_indexypb;	/* LDA ($xx),y	*/
	cpu_address_modes[0xB4] = cpu_address_zpx; 		/* LDY $xx,x	*/
	cpu_address_modes[0xB5] = cpu_address_zpx; 		/* LDA $xx,x	*/
	cpu_address_modes[0xB6] = cpu_address_zpy; 		/* LDX $xx,y	*/
	cpu_address_modes[0xB8] = cpu_address_dummy;	/* CLV			*/
	cpu_address_modes[0xB9] = cpu_address_absypb; 	/* LDA $xxxx,y	*/
	cpu_address_modes[0xBA] = cpu_address_dummy;	/* TSX	 		*/
	cpu_address_modes[0xBC] = cpu_address_absxpb; 	/* LDY $xxxx,x	*/
	cpu_address_modes[0xBD] = cpu_address_absxpb; 	/* LDA $xxxx,x	*/
	cpu_address_modes[0xBE] = cpu_address_absypb; 	/* LDX $xxxx,y	*/
	cpu_address_modes[0xC0] = cpu_address_imm;	 	/* CPY #xx		*/
	cpu_address_modes[0xC1] = cpu_address_indexx; 	/* CMP ($xx,x)	*/
	cpu_address_modes[0xC4] = cpu_address_zp;	 	/* CPY $xx		*/
	cpu_address_modes[0xC5] = cpu_address_zp;	 	/* CMP $xx		*/
	cpu_address_modes[0xC6] = cpu_address_zp;	 	/* DEC $xx		*/
	cpu_address_modes[0xC8] = cpu_address_dummy;	/* INY			*/
	cpu_address_modes[0xC9] = cpu_address_imm;	 	/* CMP #xx		*/
	cpu_address_modes[0xCA] = cpu_address_dummy; 	/* DEX			*/
	cpu_address_modes[0xCC] = cpu_address_abs;	 	/* CPY $xxxx	*/
	cpu_address_modes[0xCD] = cpu_address_abs;	 	/* CMP $xxxx	*/
	cpu_address_modes[0xCE] = cpu_address_abs;	 	/* DEC $xxxx	*/
	cpu_address_modes[0xD0] = cpu_address_rel;	 	/* BNE $xx		*/
	cpu_address_modes[0xD1] = cpu_address_indexy; 	/* CMP ($xx),y	*/
	cpu_address_modes[0xD5] = cpu_address_zpx;	 	/* CMP $xx,x	*/
	cpu_address_modes[0xD6] = cpu_address_zpx;	 	/* DEC $xx,x	*/
	cpu_address_modes[0xD8] = cpu_address_dummy;	/* CLD			*/
	cpu_address_modes[0xD9] = cpu_address_absypb; 	/* CMP $xxxx,y	*/
	cpu_address_modes[0xDD] = cpu_address_absxpb; 	/* CMP $xxxx,x	*/
	cpu_address_modes[0xDE] = cpu_address_absx;	 	/* DEC $xxxx,x	*/
	cpu_address_modes[0xE0] = cpu_address_imm;	 	/* CPX #xx		*/
	cpu_address_modes[0xE1] = cpu_address_indexx; 	/* SBC ($xx,x)	*/
	cpu_address_modes[0xE4] = cpu_address_zp;	 	/* CPX $xx		*/
	cpu_address_modes[0xE5] = cpu_address_zp;	 	/* SBC $xx		*/
	cpu_address_modes[0xE6] = cpu_address_zp;	 	/* INC $xx		*/
	cpu_address_modes[0xE8] = cpu_address_dummy;	/* INX			*/
	cpu_address_modes[0xE9] = cpu_address_imm;	 	/* SBC #xx		*/
	cpu_address_modes[0xEA] = cpu_address_dummy; 	/* NOP			*/
	cpu_address_modes[0xEC] = cpu_address_abs;	 	/* CPX $xxxx	*/
	cpu_address_modes[0xED] = cpu_address_abs;	 	/* SBC $xxxx	*/
	cpu_address_modes[0xEE] = cpu_address_abs;	 	/* INC $xxxx	*/
	cpu_address_modes[0xF0] = cpu_address_rel;	 	/* BEQ $xx		*/
	cpu_address_modes[0xF1] = cpu_address_indexy; 	/* SBC ($xx),y	*/
	cpu_address_modes[0xF5] = cpu_address_zpx;	 	/* SBC $xx,x	*/
	cpu_address_modes[0xF6] = cpu_address_zpx;	 	/* INC $xx,x	*/
	cpu_address_modes[0xF8] = cpu_address_dummy;	/* SED			*/
	cpu_address_modes[0xF9] = cpu_address_absypb; 	/* SBC $xxxx,y	*/
	cpu_address_modes[0xFD] = cpu_address_absxpb; 	/* SBC $xxxx,x	*/
	cpu_address_modes[0xFE] = cpu_address_absx;	 	/* INC $xxxx,x	*/
}

word cpu_address_imm(nes_t *nes)
{
	/* Get value according to the PC of the CPU */
	word address = cpu_get_pc(nes_get_cpu(nes)) + 1;
	return address;
}

word cpu_address_zp(nes_t *nes)
{
	/* Get the value from zero page that PC + 1 points to */
	word address = cpu_get_pc(nes_get_cpu(nes)) + 1;
	address = nes_cpu_mem_read(nes, address);
	return address;
}

word cpu_address_zpx(nes_t *nes)
{
	/* Get the value from zero page that PC + 1 points to */
	word address = cpu_get_pc(nes_get_cpu(nes)) + 1;

	/* Add the X register to that address */
	address = nes_cpu_mem_read(nes, address);
	address += cpu_get_x(nes_get_cpu(nes));

	/* Make sure we stay on zero page */
	address &= 0x00FF;
	return address;
}

word cpu_address_zpy(nes_t *nes)
{
	/* Get the value from zero page that PC + 1 points to */
	word address = cpu_get_pc(nes_get_cpu(nes)) + 1;

	/* Add the Y register to that address */
	address = nes_cpu_mem_read(nes, address);
	address += cpu_get_y(nes_get_cpu(nes));

	/* Make sure we stay on zero page */
	address &= 0x00FF;
	return address;
}

word cpu_address_abs(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;
	return address;
}

word cpu_address_ind(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;

	/* Now get the destination address */
	word dest = nes_cpu_mem_read(nes, address);
	dest += (nes_cpu_mem_read(nes, address + 1) << 8);

	return dest;
}

word cpu_address_absx(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;

	/* Add the X register to that address */
	address += cpu_get_x(nes_get_cpu(nes));
	return address;
}

word cpu_address_absxpb(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;

	/* Add the X register to our address. If we cross a page boundary, add a clock cycle */
	word final_address = address + cpu_get_x(nes_get_cpu(nes));
	if((address & 0xFF00) != (final_address & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);

	return final_address;
}


word cpu_address_absy(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;

	/* Add the Y register to that address */
	address += cpu_get_y(nes_get_cpu(nes));
	return address;
}

word cpu_address_absypb(nes_t *nes)
{
	/* Get the value from memory that the word that follows the PC points to */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	word address = nes_cpu_mem_read(nes, pc + 1);
	address += nes_cpu_mem_read(nes, pc + 2) << 8;

	/* Add the Y register to that address. If we cross a page boundary, add a clock cycle */
	word final_address = address + cpu_get_y(nes_get_cpu(nes));
	if((address & 0xFF00) != (final_address & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);

	return final_address;
}

word cpu_address_indexx(nes_t *nes)
{
	/* Get the value from zero page that PC + 1 points to */
	word pc = cpu_get_pc(nes_get_cpu(nes)) + 1;
	word zp_address = nes_cpu_mem_read(nes, pc);

	/* Add the X register to the address */
	zp_address += cpu_get_x(nes_get_cpu(nes));
	/* Make sure we _stay_ on zero page */
	zp_address &= 0x00FF;

	/* The final address is fetched from zero page */
	word final_address = nes_cpu_mem_read(nes, zp_address);
	final_address += nes_cpu_mem_read(nes, zp_address + 1) << 8;
	return final_address;
	
}

word cpu_address_indexy(nes_t *nes)
{
	
	/* Get the value from zero page that PC + 1 points to */
	word pc = cpu_get_pc(nes_get_cpu(nes)) + 1;
	word zp_address = nes_cpu_mem_read(nes, pc);

	/* Fetch the zero page address */
	word final_address = nes_cpu_mem_read(nes, zp_address);
	final_address += nes_cpu_mem_read(nes, zp_address + 1) << 8;

	/* Add the value of the Y register to the address */
	final_address += cpu_get_y(nes_get_cpu(nes));

	return final_address;
}

word cpu_address_indexypb(nes_t *nes)
{
	
	/* Get the value from zero page that PC + 1 points to */
	word pc = cpu_get_pc(nes_get_cpu(nes)) + 1;
	word zp_address = nes_cpu_mem_read(nes, pc);

	/* Fetch the zero page address */
	word final_address = nes_cpu_mem_read(nes, zp_address);
	final_address += nes_cpu_mem_read(nes, zp_address + 1) << 8;

	/* Add the value of the Y register to the address. If we
	   cross a page boundary, add a clock cycle */
	word final_address2 = final_address + cpu_get_y(nes_get_cpu(nes));
	if((final_address2 & 0xFF00) != (final_address & 0xFF00))
		cpu_add_cycles(nes_get_cpu(nes), 1);

	return final_address2;
}

word cpu_address_rel(nes_t *nes)
{
	/* The byte after the instruction is an offset potentially to be added to the PC */
	word offset = cpu_get_pc(nes_get_cpu(nes)) + 1;

	return offset;
}

word cpu_address_dummy(nes_t *nes)
{
	(void) nes;
	return 0;
}

