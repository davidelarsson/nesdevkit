#include "defines.h"
#include "cpu.h"
#include "cpu_addressing.h"
#include "cpu_instructions.h"
#include <stdlib.h>

cpu_t *cpu_init()
{
	cpu_t *cpu = malloc(sizeof(cpu_t));
	cpu->a = 0x00;
	cpu->x = 0x00;
	cpu->y = 0x00;
	cpu->p = 0x20;	/* Bit 5 is always set */
	cpu->sp = 0xFF;
	cpu->pc = 0x8000;
	cpu->cycles = 0;

	/* Initialize the cpu_address_array */
	cpu_address_init();
	cpu_opcodes_init();
	cpu_cycles_init();
	cpu_instruction_length_init();

	return cpu;

}

void cpu_cycles_init()
{
	for(int i = 0; i < 256; i++)
		cpu_cycles[i] = 0;
	
	cpu_cycles[0x00] = 0x07;	/* BRK			*/
	cpu_cycles[0x01] = 0x06;	/* ORA ($xx,x)	*/
	cpu_cycles[0x05] = 0x03;	/* ORA $xx		*/
	cpu_cycles[0x06] = 0x05;	/* ASL $xx		*/
	cpu_cycles[0x08] = 0x03;	/* PHP			*/
	cpu_cycles[0x09] = 0x02;	/* ORA #xx		*/
	cpu_cycles[0x0A] = 0x02;	/* ASL A		*/
	cpu_cycles[0x0D] = 0x04;	/* ORA $xxxx	*/
	cpu_cycles[0x0E] = 0x06;	/* ASL $xxxx	*/
	cpu_cycles[0x10] = 0x02;	/* BPL $xx		*/
	cpu_cycles[0x11] = 0x05;	/* ORA ($xx),y	*/
	cpu_cycles[0x15] = 0x04;	/* ORA $xx,x	*/
	cpu_cycles[0x16] = 0x06;	/* ASL $xx,x	*/
	cpu_cycles[0x18] = 0x02;	/* CLC			*/
	cpu_cycles[0x19] = 0x04;	/* ORA $xxxx,y	*/
	cpu_cycles[0x1D] = 0x04;	/* ORA $xxxx,x	*/
	cpu_cycles[0x1E] = 0x07;	/* ASL $xxxx,x	*/
	cpu_cycles[0x20] = 0x06;	/* JSR $xxxx	*/
	cpu_cycles[0x21] = 0x06;	/* AND ($xx,x)	*/
	cpu_cycles[0x24] = 0x03;	/* BIT $xx		*/
	cpu_cycles[0x25] = 0x03;	/* AND $xx		*/
	cpu_cycles[0x26] = 0x05;	/* ROL $xx		*/
	cpu_cycles[0x28] = 0x04;	/* PLP			*/
	cpu_cycles[0x29] = 0x02;	/* AND #xx		*/
	cpu_cycles[0x2A] = 0x02;	/* ROL A		*/
	cpu_cycles[0x2C] = 0x04;	/* BIT $xxxx	*/
	cpu_cycles[0x2D] = 0x04;	/* AND $xxxx	*/
	cpu_cycles[0x2E] = 0x06;	/* ROL $xxxx	*/
	cpu_cycles[0x30] = 0x02;	/* BMI $xx		*/
	cpu_cycles[0x31] = 0x05;	/* AND ($xx),y	*/
	cpu_cycles[0x35] = 0x04;	/* AND $xx,x	*/
	cpu_cycles[0x36] = 0x06;	/* ROL $xx,x	*/
	cpu_cycles[0x38] = 0x02;	/* SEC			*/
	cpu_cycles[0x39] = 0x04;	/* AND $xxxx,y	*/
	cpu_cycles[0x3D] = 0x04;	/* AND $xxxx,x	*/
	cpu_cycles[0x3E] = 0x07;	/* ROL $xxxx,x	*/
	cpu_cycles[0x40] = 0x06;	/* RTI			*/
	cpu_cycles[0x41] = 0x06;	/* EOR ($xx,x)	*/
	cpu_cycles[0x45] = 0x03;	/* EOR $xx		*/
	cpu_cycles[0x46] = 0x05;	/* LSR $xx		*/
	cpu_cycles[0x48] = 0x03;	/* PHA			*/
	cpu_cycles[0x49] = 0x02;	/* EOR #xx		*/
	cpu_cycles[0x4A] = 0x02;	/* LSR A		*/
	cpu_cycles[0x4C] = 0x03;	/* JMP $xxxx	*/
	cpu_cycles[0x4D] = 0x04;	/* EOR $xxxx	*/
	cpu_cycles[0x4E] = 0x06;	/* LSR $xxxx	*/
	cpu_cycles[0x50] = 0x02;	/* BVC $xx		*/
	cpu_cycles[0x51] = 0x05;	/* EOR ($xx),y	*/
	cpu_cycles[0x55] = 0x04;	/* EOR $xx,x	*/
	cpu_cycles[0x56] = 0x06;	/* LSR $xx,x	*/
	cpu_cycles[0x58] = 0x02;	/* CLI			*/
	cpu_cycles[0x59] = 0x04;	/* EOR $xxxx,y	*/
	cpu_cycles[0x5D] = 0x04;	/* EOR $xxxx,x	*/
	cpu_cycles[0x5E] = 0x07;	/* LSR $xxxx,x	*/
	cpu_cycles[0x60] = 0x06;	/* RTS			*/
	cpu_cycles[0x61] = 0x06;	/* ADC ($xx,x)	*/
	cpu_cycles[0x65] = 0x03;	/* ADC $xx		*/
	cpu_cycles[0x66] = 0x05;	/* ROR $xx		*/
	cpu_cycles[0x68] = 0x04;	/* PLA			*/
	cpu_cycles[0x69] = 0x02;	/* ADC #xx		*/
	cpu_cycles[0x6A] = 0x02;	/* ROR A		*/
	cpu_cycles[0x6C] = 0x05;	/* JMP ($xxxx)	*/
	cpu_cycles[0x6D] = 0x04;	/* ADC $xxxx	*/
	cpu_cycles[0x6E] = 0x06;	/* ROR $xxxx	*/
	cpu_cycles[0x70] = 0x02;	/* BVS $xx		*/
	cpu_cycles[0x71] = 0x05;	/* ADC ($xx),y	*/
	cpu_cycles[0x75] = 0x04;	/* ADC $xx,x	*/
	cpu_cycles[0x76] = 0x06;	/* ROR $xx,x	*/
	cpu_cycles[0x78] = 0x02;	/* SEI			*/
	cpu_cycles[0x79] = 0x04;	/* ADC $xxxx,y	*/
	cpu_cycles[0x7D] = 0x04;	/* ADC $xxxx,x	*/
	cpu_cycles[0x7E] = 0x07;	/* ROR $xxxx,x	*/
	cpu_cycles[0x81] = 0x06;	/* STA ($xx,x)	*/
	cpu_cycles[0x84] = 0x03;	/* STY $xx		*/
	cpu_cycles[0x85] = 0x03;	/* STA $xx		*/
	cpu_cycles[0x86] = 0x03;	/* STX $xx		*/
	cpu_cycles[0x88] = 0x02;	/* DEY			*/
	cpu_cycles[0x8A] = 0x02;	/* TXA			*/
	cpu_cycles[0x8C] = 0x04;	/* STY $xxxx	*/
	cpu_cycles[0x8D] = 0x04;	/* STA $xxxx	*/
	cpu_cycles[0x8E] = 0x04;	/* STX $xxxx	*/
	cpu_cycles[0x90] = 0x02;	/* BCC $xx		*/
	cpu_cycles[0x91] = 0x06;	/* STA ($xx),y	*/
	cpu_cycles[0x94] = 0x04;	/* STY $xx,x	*/
	cpu_cycles[0x95] = 0x04;	/* STA $xx,x	*/
	cpu_cycles[0x96] = 0x04;	/* STX $xx,y	*/
	cpu_cycles[0x98] = 0x02;	/* TYA			*/
	cpu_cycles[0x99] = 0x05;	/* STA $xxxx,y	*/
	cpu_cycles[0x9A] = 0x02;	/* TXS			*/
	cpu_cycles[0x9D] = 0x05;	/* STA $xxxx,x	*/
	cpu_cycles[0xA0] = 0x02;	/* LDY #xx		*/
	cpu_cycles[0xA1] = 0x06;	/* LDA ($xx,x)	*/
	cpu_cycles[0xA2] = 0x02;	/* LDX #xx		*/
	cpu_cycles[0xA4] = 0x03;	/* LDY $xx		*/
	cpu_cycles[0xA5] = 0x03;	/* LDA $xx		*/
	cpu_cycles[0xA6] = 0x03;	/* LDX $xx		*/
	cpu_cycles[0xA8] = 0x02;	/* TAY			*/
	cpu_cycles[0xA9] = 0x02;	/* LDA #xx		*/
	cpu_cycles[0xAA] = 0x02;	/* TAX			*/
	cpu_cycles[0xAC] = 0x04;	/* LDY $xxxx	*/
	cpu_cycles[0xAD] = 0x04;	/* LDA $xxxx	*/
	cpu_cycles[0xAE] = 0x04;	/* LDX $xxxx	*/
	cpu_cycles[0xB0] = 0x02;	/* BCS $xx		*/
	cpu_cycles[0xB1] = 0x05;	/* LDA ($xx),y	*/
	cpu_cycles[0xB4] = 0x04;	/* LDY $xx,x	*/
	cpu_cycles[0xB5] = 0x04;	/* LDA $xx,x	*/
	cpu_cycles[0xB6] = 0x04;	/* LDX $xx,y	*/
	cpu_cycles[0xB8] = 0x02;	/* CLV			*/
	cpu_cycles[0xB9] = 0x04;	/* LDA $xxxx,y	*/
	cpu_cycles[0xBA] = 0x02;	/* TSX			*/
	cpu_cycles[0xBC] = 0x04;	/* LDY $xxxx,x	*/
	cpu_cycles[0xBD] = 0x04;	/* LDA $xxxx,x	*/
	cpu_cycles[0xBE] = 0x04;	/* LDX $xxxx,y	*/
	cpu_cycles[0xC0] = 0x02;	/* CPY #xx		*/
	cpu_cycles[0xC1] = 0x06;	/* CMP ($xx,x)	*/
	cpu_cycles[0xC4] = 0x03;	/* CPY $xx		*/
	cpu_cycles[0xC5] = 0x03;	/* CMP $xx		*/
	cpu_cycles[0xC6] = 0x05;	/* DEC $xx		*/
	cpu_cycles[0xC8] = 0x02;	/* INY			*/
	cpu_cycles[0xC9] = 0x02;	/* CMP #xx		*/
	cpu_cycles[0xCA] = 0x02;	/* DEX			*/
	cpu_cycles[0xCC] = 0x04;	/* CPY $xxxx	*/
	cpu_cycles[0xCD] = 0x04;	/* CMP $xxxx	*/
	cpu_cycles[0xCE] = 0x06;	/* DEC $xxxx	*/
	cpu_cycles[0xD0] = 0x02;	/* BNE $xx		*/
	cpu_cycles[0xD1] = 0x05;	/* CMP ($xx),y	*/
	cpu_cycles[0xD5] = 0x04;	/* CMP $xx,x	*/
	cpu_cycles[0xD6] = 0x06;	/* DEC $xx,x	*/
	cpu_cycles[0xD8] = 0x02;	/* CLD			*/
	cpu_cycles[0xD9] = 0x04;	/* CMP $xxxx,y	*/
	cpu_cycles[0xDD] = 0x04;	/* CMP $xxxx,x	*/
	cpu_cycles[0xDE] = 0x07;	/* DEC $xxxx,x	*/
	cpu_cycles[0xE0] = 0x02;	/* CPX #xx		*/
	cpu_cycles[0xE1] = 0x06;	/* SBC ($xx,x)	*/
	cpu_cycles[0xE4] = 0x03;	/* CPX $xx		*/
	cpu_cycles[0xE5] = 0x03;	/* SBC $xx		*/
	cpu_cycles[0xE6] = 0x05;	/* INC $xx		*/
	cpu_cycles[0xE8] = 0x02;	/* INX			*/
	cpu_cycles[0xE9] = 0x02;	/* SBC #xx		*/
	cpu_cycles[0xEA] = 0x02;	/* NOP			*/
	cpu_cycles[0xEC] = 0x04;	/* CPX $xxxx	*/
	cpu_cycles[0xED] = 0x04;	/* SBC $xxxx	*/
	cpu_cycles[0xEE] = 0x06;	/* INC $xxxx	*/
	cpu_cycles[0xF0] = 0x02;	/* BEQ $xx		*/
	cpu_cycles[0xF1] = 0x05;	/* SBC ($xx),y	*/
	cpu_cycles[0xF5] = 0x04;	/* SBC $xx,x	*/
	cpu_cycles[0xF6] = 0x06;	/* INC $xx,x	*/
	cpu_cycles[0xF8] = 0x02;	/* SED			*/
	cpu_cycles[0xF9] = 0x04;	/* SBC $xxxx,y	*/
	cpu_cycles[0xFD] = 0x04;	/* SBC $xxxx,x	*/
	cpu_cycles[0xFE] = 0x07;	/* INC $xxxx,x	*/
}

void cpu_instruction_length_init()
{
	/* Assume that all unimplemented opcodes are one byte of length */
	for(int i = 0; i < 256; i++)
		cpu_instruction_length[i] = 1;

	cpu_instruction_length[0x00] = 0x00;	/* BRK			*/ 
	cpu_instruction_length[0x01] = 0x02;	/* ORA ($xx,x)	*/
	cpu_instruction_length[0x05] = 0x02;	/* ORA $xx		*/
	cpu_instruction_length[0x06] = 0x02;	/* ASL $xx		*/
	cpu_instruction_length[0x08] = 0x01;	/* PHP			*/
	cpu_instruction_length[0x09] = 0x02;	/* ORA #xx		*/
	cpu_instruction_length[0x0A] = 0x01;	/* ASL A		*/
	cpu_instruction_length[0x0D] = 0x03;	/* ORA $xxxx	*/
	cpu_instruction_length[0x0E] = 0x03;	/* ASL $xxxx	*/
	cpu_instruction_length[0x10] = 0x00;	/* BPL $xx		*/
	cpu_instruction_length[0x11] = 0x02;	/* ORA ($xx),y	*/
	cpu_instruction_length[0x15] = 0x02;	/* ORA $xx,x	*/
	cpu_instruction_length[0x16] = 0x02;	/* ASL $xx,x	*/
	cpu_instruction_length[0x18] = 0x01;	/* CLC			*/
	cpu_instruction_length[0x19] = 0x03;	/* ORA $xxxx,y	*/
	cpu_instruction_length[0x1D] = 0x03;	/* ORA $xxxx,x	*/
	cpu_instruction_length[0x1E] = 0x03;	/* ASL $xxxx,x	*/
	cpu_instruction_length[0x20] = 0x00;	/* JSR $xxxx	*/
	cpu_instruction_length[0x21] = 0x02;	/* AND ($xx,x)	*/
	cpu_instruction_length[0x24] = 0x02;	/* BIT $xx		*/
	cpu_instruction_length[0x25] = 0x02;	/* AND $xx		*/
	cpu_instruction_length[0x26] = 0x02;	/* ROL $xx		*/
	cpu_instruction_length[0x28] = 0x01;	/* PLP			*/
	cpu_instruction_length[0x29] = 0x02;	/* AND #xx		*/
	cpu_instruction_length[0x2A] = 0x01;	/* ROL A		*/
	cpu_instruction_length[0x2C] = 0x03;	/* BIT $xxxx	*/
	cpu_instruction_length[0x2D] = 0x03;	/* AND $xxxx	*/
	cpu_instruction_length[0x2E] = 0x03;	/* ROL $xxxx	*/
	cpu_instruction_length[0x30] = 0x00;	/* BMI $xx		*/
	cpu_instruction_length[0x31] = 0x02;	/* AND ($xx),y	*/
	cpu_instruction_length[0x35] = 0x02;	/* AND $xx,x	*/
	cpu_instruction_length[0x36] = 0x02;	/* ROL $xx,x	*/
	cpu_instruction_length[0x38] = 0x01;	/* SEC			*/
	cpu_instruction_length[0x39] = 0x03;	/* AND $xxxx,y	*/
	cpu_instruction_length[0x3D] = 0x03;	/* AND $xxxx,x	*/
	cpu_instruction_length[0x3E] = 0x03;	/* ROL $xxxx,x	*/
	cpu_instruction_length[0x40] = 0x00;	/* RTI			*/
	cpu_instruction_length[0x41] = 0x02;	/* EOR ($xx,x)	*/
	cpu_instruction_length[0x45] = 0x02;	/* EOR $xx		*/
	cpu_instruction_length[0x46] = 0x02;	/* LSR $xx		*/
	cpu_instruction_length[0x48] = 0x01;	/* PHA			*/
	cpu_instruction_length[0x49] = 0x02;	/* EOR #xx		*/
	cpu_instruction_length[0x4A] = 0x01;	/* LSR A		*/
	cpu_instruction_length[0x4C] = 0x00;	/* JMP $xxxx	*/
	cpu_instruction_length[0x4D] = 0x03;	/* EOR $xxxx	*/
	cpu_instruction_length[0x4E] = 0x03;	/* LSR $xxxx	*/
	cpu_instruction_length[0x50] = 0x00;	/* BVC $xx		*/
	cpu_instruction_length[0x51] = 0x02;	/* EOR ($xx),y	*/
	cpu_instruction_length[0x55] = 0x02;	/* EOR $xx,x	*/
	cpu_instruction_length[0x56] = 0x02;	/* LSR $xx,x	*/
	cpu_instruction_length[0x58] = 0x01;	/* CLI			*/
	cpu_instruction_length[0x59] = 0x03;	/* EOR $xxxx,y	*/
	cpu_instruction_length[0x5D] = 0x03;	/* EOR $xxxx,x	*/
	cpu_instruction_length[0x5E] = 0x03;	/* LSR $xxxx,x	*/
	cpu_instruction_length[0x60] = 0x00;	/* RTS			*/
	cpu_instruction_length[0x61] = 0x02;	/* ADC ($xx,x)	*/
	cpu_instruction_length[0x65] = 0x02;	/* ADC $xx		*/
	cpu_instruction_length[0x66] = 0x02;	/* ROR $xx		*/
	cpu_instruction_length[0x68] = 0x01;	/* PLA			*/
	cpu_instruction_length[0x69] = 0x02;	/* ADC #xx		*/
	cpu_instruction_length[0x6A] = 0x01;	/* ROR A		*/
	cpu_instruction_length[0x6C] = 0x00;	/* JMP ($xxxx)	*/
	cpu_instruction_length[0x6D] = 0x03;	/* ADC $xxxx	*/
	cpu_instruction_length[0x6E] = 0x03;	/* ROR $xxxx	*/
	cpu_instruction_length[0x70] = 0x00;	/* BVS $xx		*/
	cpu_instruction_length[0x71] = 0x02;	/* ADC ($xx),y	*/
	cpu_instruction_length[0x75] = 0x02;	/* ADC $xx,x	*/
	cpu_instruction_length[0x76] = 0x02;	/* ROR $xx,x	*/
	cpu_instruction_length[0x78] = 0x01;	/* SEI			*/
	cpu_instruction_length[0x79] = 0x03;	/* ADC $xxxx,y	*/
	cpu_instruction_length[0x7D] = 0x03;	/* ADC $xxxx,x	*/
	cpu_instruction_length[0x7E] = 0x03;	/* ROR $xxxx,x	*/
	cpu_instruction_length[0x81] = 0x02;	/* STA ($xx,x)	*/
	cpu_instruction_length[0x84] = 0x02;	/* STY $xx		*/
	cpu_instruction_length[0x85] = 0x02;	/* STA $xx		*/
	cpu_instruction_length[0x86] = 0x02;	/* STX $xx		*/
	cpu_instruction_length[0x88] = 0x01;	/* DEY			*/
	cpu_instruction_length[0x8A] = 0x01;	/* TXA			*/
	cpu_instruction_length[0x8C] = 0x03;	/* STY $xxxx	*/
	cpu_instruction_length[0x8D] = 0x03;	/* STA $xxxx	*/
	cpu_instruction_length[0x8E] = 0x03;	/* STX $xxxx	*/
	cpu_instruction_length[0x90] = 0x00;	/* BCC $xx		*/
	cpu_instruction_length[0x91] = 0x02;	/* STA ($xx),y	*/
	cpu_instruction_length[0x94] = 0x02;	/* STY $xx,x	*/
	cpu_instruction_length[0x95] = 0x02;	/* STA $xx,x	*/
	cpu_instruction_length[0x96] = 0x02;	/* STX $xx,y	*/
	cpu_instruction_length[0x98] = 0x01;	/* TYA			*/
	cpu_instruction_length[0x99] = 0x03;	/* STA $xxxx,y	*/
	cpu_instruction_length[0x9A] = 0x01;	/* TXS			*/
	cpu_instruction_length[0x9D] = 0x03;	/* STA $xxxx,x	*/
	cpu_instruction_length[0xA0] = 0x02;	/* LDY #xx		*/
	cpu_instruction_length[0xA1] = 0x02;	/* LDA ($xx,x)	*/
	cpu_instruction_length[0xA2] = 0x02;	/* LDX #xx		*/
	cpu_instruction_length[0xA4] = 0x02;	/* LDY $xx		*/
	cpu_instruction_length[0xA5] = 0x02;	/* LDA $xx		*/
	cpu_instruction_length[0xA6] = 0x02;	/* LDX $xx		*/
	cpu_instruction_length[0xA8] = 0x01;	/* TAY			*/
	cpu_instruction_length[0xA9] = 0x02;	/* LDA #xx		*/
	cpu_instruction_length[0xAA] = 0x01;	/* TAX			*/
	cpu_instruction_length[0xAC] = 0x03;	/* LDY $xxxx	*/
	cpu_instruction_length[0xAD] = 0x03;	/* LDA $xxxx	*/
	cpu_instruction_length[0xAE] = 0x03;	/* LDX $xxxx	*/
	cpu_instruction_length[0xB0] = 0x00;	/* BCS $xx		*/
	cpu_instruction_length[0xB1] = 0x02;	/* LDA ($xx),y	*/
	cpu_instruction_length[0xB4] = 0x02;	/* LDY $xx,x	*/
	cpu_instruction_length[0xB5] = 0x02;	/* LDA $xx,x	*/
	cpu_instruction_length[0xB6] = 0x02;	/* LDX $xx,y	*/
	cpu_instruction_length[0xB8] = 0x01;	/* CLV			*/
	cpu_instruction_length[0xB9] = 0x03;	/* LDA $xxxx,y	*/
	cpu_instruction_length[0xBA] = 0x01;	/* TSX			*/
	cpu_instruction_length[0xBC] = 0x03;	/* LDY $xxxx,x	*/
	cpu_instruction_length[0xBD] = 0x03;	/* LDA $xxxx,x	*/
	cpu_instruction_length[0xBE] = 0x03;	/* LDX $xxxx,y	*/
	cpu_instruction_length[0xC0] = 0x02;	/* CPY #xx		*/
	cpu_instruction_length[0xC1] = 0x02;	/* CMP ($xx,x)	*/
	cpu_instruction_length[0xC4] = 0x02;	/* CPY $xx		*/
	cpu_instruction_length[0xC5] = 0x02;	/* CMP $xx		*/
	cpu_instruction_length[0xC6] = 0x02;	/* DEC $xx		*/
	cpu_instruction_length[0xC8] = 0x01;	/* INY			*/
	cpu_instruction_length[0xC9] = 0x02;	/* CMP #xx		*/
	cpu_instruction_length[0xCA] = 0x01;	/* DEX			*/
	cpu_instruction_length[0xCC] = 0x03;	/* CPY $xxxx	*/
	cpu_instruction_length[0xCD] = 0x03;	/* CMP $xxxx	*/
	cpu_instruction_length[0xCE] = 0x03;	/* DEC $xxxx	*/
	cpu_instruction_length[0xD0] = 0x00;	/* BNE $xx		*/
	cpu_instruction_length[0xD1] = 0x02;	/* CMP ($xx),y	*/
	cpu_instruction_length[0xD5] = 0x02;	/* CMP $xx,x	*/
	cpu_instruction_length[0xD6] = 0x02;	/* DEC $xx,x	*/
	cpu_instruction_length[0xD8] = 0x01;	/* CLD			*/
	cpu_instruction_length[0xD9] = 0x03;	/* CMP $xxxx,y	*/
	cpu_instruction_length[0xDD] = 0x03;	/* CMP $xxxx,x	*/
	cpu_instruction_length[0xDE] = 0x03;	/* DEC $xxxx,x	*/
	cpu_instruction_length[0xE0] = 0x02;	/* CPX #xx		*/
	cpu_instruction_length[0xE1] = 0x02;	/* SBC ($xx,x)	*/
	cpu_instruction_length[0xE4] = 0x02;	/* CPX $xx		*/
	cpu_instruction_length[0xE5] = 0x02;	/* SBC $xx		*/
	cpu_instruction_length[0xE6] = 0x02;	/* INC $xx		*/
	cpu_instruction_length[0xE8] = 0x01;	/* INX			*/
	cpu_instruction_length[0xE9] = 0x02;	/* SBC #xx		*/
	cpu_instruction_length[0xEA] = 0x01;	/* NOP			*/
	cpu_instruction_length[0xEC] = 0x03;	/* CPX $xxxx	*/
	cpu_instruction_length[0xED] = 0x03;	/* SBC $xxxx	*/
	cpu_instruction_length[0xEE] = 0x03;	/* INC $xxxx	*/
	cpu_instruction_length[0xF0] = 0x00;	/* BEQ $xx		*/
	cpu_instruction_length[0xF1] = 0x02;	/* SBC ($xx),y	*/
	cpu_instruction_length[0xF5] = 0x02;	/* SBC $xx,x	*/
	cpu_instruction_length[0xF6] = 0x02;	/* INC $xx,x	*/
	cpu_instruction_length[0xF8] = 0x01;	/* SED			*/
	cpu_instruction_length[0xF9] = 0x03;	/* SBC $xxxx,y	*/
	cpu_instruction_length[0xFD] = 0x03;	/* SBC $xxxx,x	*/
	cpu_instruction_length[0xFE] = 0x03;	/* INC $xxxx,x	*/
}

void cpu_destroy(cpu_t *cpu)
{
	free(cpu);
}

void cpu_add_cycles(cpu_t *cpu, unsigned cycles)
{
	cpu->cycles += cycles;
}	

unsigned long long cpu_get_cycles(cpu_t *cpu)
{
	return cpu->cycles;
}

byte cpu_get_a(cpu_t *cpu)
{
	return cpu->a;
}

byte cpu_get_x(cpu_t *cpu)
{
	return cpu->x;
}

byte cpu_get_y(cpu_t *cpu)
{
	return cpu->y;
}

byte cpu_get_sp(cpu_t *cpu)
{
	return cpu->sp;
}

byte cpu_get_p(cpu_t *cpu)
{
	return cpu->p;
}

word cpu_get_pc(cpu_t *cpu)
{
	return cpu->pc;
}

void cpu_set_a(cpu_t *cpu, byte val)
{
	cpu->a = val;
}

void cpu_set_x(cpu_t *cpu, byte val)
{
	cpu->x = val;
}

void cpu_set_y(cpu_t *cpu, byte val)
{
	cpu->y = val;
}

void cpu_set_sp(cpu_t *cpu, byte address)
{
	cpu->sp = address;
}

void cpu_set_p(cpu_t *cpu, byte flags)
{
	cpu->p = flags | 0x20; /* Bit 5 is always set */
}

void cpu_set_pc(cpu_t *cpu, word address)
{
	cpu->pc = address;
}

void cpu_set_c(cpu_t *cpu)
{
	cpu->p |= MASK_CARRY;
}

void cpu_set_z(cpu_t *cpu)
{
	cpu->p |= MASK_ZERO;
}

void cpu_set_i(cpu_t *cpu)
{
	cpu->p |= MASK_INTERRUPT;
}

void cpu_set_d(cpu_t *cpu)
{
	cpu->p |= MASK_DECIMAL;
}

void cpu_set_b(cpu_t *cpu)
{
	cpu->p |= MASK_BREAK;
}

void cpu_set_v(cpu_t *cpu)
{
	cpu->p |= MASK_OVERFLOW;
}

void cpu_set_n(cpu_t *cpu)
{
	cpu->p |= MASK_NEGATIVE;
}

void cpu_clear_c(cpu_t *cpu)
{
	cpu->p &= ~MASK_CARRY;
}

void cpu_clear_z(cpu_t *cpu)
{
	cpu->p &= ~MASK_ZERO;
}

void cpu_clear_i(cpu_t *cpu)
{
	cpu->p &= ~MASK_INTERRUPT;
}

void cpu_clear_d(cpu_t *cpu)
{
	cpu->p &= ~MASK_DECIMAL;
}

void cpu_clear_b(cpu_t *cpu)
{
	cpu->p &= ~MASK_BREAK;
}

void cpu_clear_v(cpu_t *cpu)
{
	cpu->p &= ~MASK_OVERFLOW;
}

void cpu_clear_n(cpu_t *cpu)
{
	cpu->p &= ~MASK_NEGATIVE;
}

bool cpu_get_c(cpu_t *cpu)
{
	return (cpu->p & MASK_CARRY);
}

bool cpu_get_z(cpu_t *cpu)
{
	return (cpu->p & MASK_ZERO);
}

bool cpu_get_i(cpu_t *cpu)
{
	return (cpu->p & MASK_INTERRUPT);
}

bool cpu_get_d(cpu_t *cpu)
{
	return (cpu->p & MASK_DECIMAL);
}

bool cpu_get_b(cpu_t *cpu)
{
	return (cpu->p & MASK_BREAK);
}

bool cpu_get_v(cpu_t *cpu)
{
	return (cpu->p & MASK_OVERFLOW);
}

bool cpu_get_n(cpu_t *cpu)
{
	return (cpu->p & MASK_NEGATIVE);
}

