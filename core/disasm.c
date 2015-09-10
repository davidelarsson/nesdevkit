#include "defines.h"
#include "disasm.h"
#include <string.h>
#include <stdio.h>

unsigned nes_disasm(byte *bytes, word pc, char *out)
{
	addressing_mode_t addressing_mode;
	unsigned len;

	switch(bytes[0]) {

		case 0x00:
			strcpy(out, "BRK");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x01:
			strcpy(out, "ORA ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x05:
			strcpy(out, "ORA ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x06:
			strcpy(out, "ASL ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x08:
			strcpy(out, "PHP");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x09:
			strcpy(out, "ORA ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0x0A:
			strcpy(out, "ASL A");
			addressing_mode = NONE;
			len = 2;
			break;

		case 0x0D:
			strcpy(out, "ORA ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x0E:
			strcpy(out, "ASL ");
			addressing_mode = ABS;
			len = 2;
			break;

		case 0x10:
			strcpy(out, "BPL ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0x11:
			strcpy(out, "ORA ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0x15:
			strcpy(out, "ORA ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x16:
			strcpy(out, "ASL ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x18:
			strcpy(out, "CLC");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x19:
			strcpy(out, "ORA ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x1D:
			strcpy(out, "ORA ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x1E:
			strcpy(out, "ASL ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x20:
			strcpy(out, "JSR ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x21:
			strcpy(out, "AND ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x24:
			strcpy(out, "BIT ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x25:
			strcpy(out, "AND ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x26:
			strcpy(out, "ROL ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x28:
			strcpy(out, "PLP");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x29:
			strcpy(out, "AND ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0x2A:
			strcpy(out, "ROL A");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x2C:
			strcpy(out, "BIT ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x2D:
			strcpy(out, "AND ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x2E:
			strcpy(out, "ROL ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x30:
			strcpy(out, "BMI ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0x31:
			strcpy(out, "AND ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0x35:
			strcpy(out, "AND ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x36:
			strcpy(out, "ROL ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x38:
			strcpy(out, "SEC");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x39:
			strcpy(out, "AND ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x3D:
			strcpy(out, "AND ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x3E:
			strcpy(out, "ROL ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x40:
			strcpy(out, "RTI");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x41:
			strcpy(out, "EOR ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x45:
			strcpy(out, "EOR ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x46:
			strcpy(out, "LSR ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x48:
			strcpy(out, "PHA");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x49:
			strcpy(out, "EOR ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0x4A:
			strcpy(out, "LSR A");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x4C:
			strcpy(out, "JMP ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x4D:
			strcpy(out, "EOR ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x4E:
			strcpy(out, "LSR ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x50:
			strcpy(out, "BVC ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0x51:
			strcpy(out, "EOR ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0x55:
			strcpy(out, "EOR ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x56:
			strcpy(out, "LSR ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x58:
			strcpy(out, "CLI");
			addressing_mode = NONE;;
			len = 1;
			break;

		case 0x59:
			strcpy(out, "EOR ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x5D:
			strcpy(out, "EOR ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x5E:
			strcpy(out, "LSR ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x60:
			strcpy(out, "RTS");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x61:
			strcpy(out, "ADC ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x65:
			strcpy(out, "ADC ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x66:
			strcpy(out, "ROR ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x68:
			strcpy(out, "PLA");
			addressing_mode = ZP;
			len = 1;
			break;

		case 0x69:
			strcpy(out, "ADC ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0x6A:
			strcpy(out, "ROR A");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x6C:
			strcpy(out, "JMP ");
			addressing_mode = IND;
			len = 3;
			break;

		case 0x6D:
			strcpy(out, "ADC ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x6E:
			strcpy(out, "ROR ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x70:
			strcpy(out, "BVS ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0x71:
			strcpy(out, "ADC ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0x75:
			strcpy(out, "ADC ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x76:
			strcpy(out, "ROR ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x78:
			strcpy(out, "SEI");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x79:
			strcpy(out, "ADC ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x7D:
			strcpy(out, "ADC ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x7E:
			strcpy(out, "ROR ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0x81:
			strcpy(out, "STA ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0x84:
			strcpy(out, "STY ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x85:
			strcpy(out, "STA ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x86:
			strcpy(out, "STX ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0x88:
			strcpy(out, "DEY");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x8A:
			strcpy(out, "TXA");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x8C:
			strcpy(out, "STY ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x8D:
			strcpy(out, "STA ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x8E:
			strcpy(out, "STX ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0x90:
			strcpy(out, "BCC ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0x91:
			strcpy(out, "STA ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0x94:
			strcpy(out, "STY ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x95:
			strcpy(out, "STA ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0x96:
			strcpy(out, "STA ");
			addressing_mode = ZPY;
			len = 2;
			break;

		case 0x98:
			strcpy(out, "TYA");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0x99:
			strcpy(out, "STA ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0x9A:
			strcpy(out, "TXS");
			addressing_mode = ABS;
			len = 1;
			break;

		case 0x9D:
			strcpy(out, "STA ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xA0:
			strcpy(out, "LDY ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xA1:
			strcpy(out, "LDA ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0xA2:
			strcpy(out, "LDX ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xA4:
			strcpy(out, "LDY ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xA5:
			strcpy(out, "LDA ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xA6:
			strcpy(out, "LDX ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xA8:
			strcpy(out, "TAY");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xA9:
			strcpy(out, "LDA ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xAA:
			strcpy(out, "TAX");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xAC:
			strcpy(out, "LDY ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xAD:
			strcpy(out, "LDA ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xAE:
			strcpy(out, "LDX ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xB0:
			strcpy(out, "BCS ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0xB1:
			strcpy(out, "LDA ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0xB4:
			strcpy(out, "LDY ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xB5:
			strcpy(out, "LDA ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xB6:
			strcpy(out, "LDX ");
			addressing_mode = ZPY;
			len = 2;
			break;

		case 0xB8:
			strcpy(out, "CLV");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xB9:
			strcpy(out, "LDA ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0xBA:
			strcpy(out, "TSX");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xBC:
			strcpy(out, "LDY ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xBD:
			strcpy(out, "LDA ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xBE:
			strcpy(out, "LDX ");
			addressing_mode = ABSY;
			len = 2;
			break;

		case 0xC0:
			strcpy(out, "CPY ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xC1:
			strcpy(out, "CMP ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0xC4:
			strcpy(out, "CPY ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xC5:
			strcpy(out, "CMP ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xC6:
			strcpy(out, "DEC ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xC8:
			strcpy(out, "INY");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xC9:
			strcpy(out, "CMP ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xCA:
			strcpy(out, "DEX ");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xCC:
			strcpy(out, "CPY ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xCD:
			strcpy(out, "CMP ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xCE:
			strcpy(out, "DEC ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xD0:
			strcpy(out, "BNE ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0xD1:
			strcpy(out, "CMP ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0xD5:
			strcpy(out, "CMP ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xD6:
			strcpy(out, "DEC ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xD8:
			strcpy(out, "CLD");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xD9:
			strcpy(out, "CMP ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0xDD:
			strcpy(out, "CMP ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xDE:
			strcpy(out, "DEC ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xE0:
			strcpy(out, "CPX ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xE1:
			strcpy(out, "SBC ");
			addressing_mode = INDEXX;
			len = 2;
			break;

		case 0xE4:
			strcpy(out, "CPX ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xE5:
			strcpy(out, "SBC ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xE6:
			strcpy(out, "INC ");
			addressing_mode = ZP;
			len = 2;
			break;

		case 0xE8:
			strcpy(out, "INX");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xE9:
			strcpy(out, "SBC ");
			addressing_mode = IMM;
			len = 2;
			break;

		case 0xEA:
			strcpy(out, "NOP");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xEC:
			strcpy(out, "CPX ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xED:
			strcpy(out, "SBC ");
			addressing_mode = ABS;
			len = 3;
			break;

		case 0xEE:
			strcpy(out, "INC ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xF0:
			strcpy(out, "BEQ ");
			addressing_mode = REL;
			len = 2;
			break;

		case 0xF1:
			strcpy(out, "SBC ");
			addressing_mode = INDEXY;
			len = 2;
			break;

		case 0xF5:
			strcpy(out, "SBC ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xF6:
			strcpy(out, "INC ");
			addressing_mode = ZPX;
			len = 2;
			break;

		case 0xF8:
			strcpy(out, "SED");
			addressing_mode = NONE;
			len = 1;
			break;

		case 0xF9:
			strcpy(out, "SBC ");
			addressing_mode = ABSY;
			len = 3;
			break;

		case 0xFD:
			strcpy(out, "SBC ");
			addressing_mode = ABSX;
			len = 3;
			break;

		case 0xFE:
			strcpy(out, "SBC ");
			addressing_mode = ABSX;
			len = 2;
			break;

		default:
			strcpy(out, "UNIMPLEMENTED!");
			addressing_mode = NONE;
			len = 1;
			break;
	}

	switch(addressing_mode) {
		case IMM:
			sprintf(out + 4, "#$%02X", bytes[1]);
			break;

		case ZP:
			sprintf(out + 4, "$%02X", bytes[1]);
			break;

		case ZPX:
			sprintf(out + 4, "$%02X,x", bytes[1]);
			break;

		case ZPY:
			sprintf(out + 4, "$%02X,y", bytes[1]);
			break;

		case ABS:
			sprintf(out + 4, "$%02X%02X", bytes[2], bytes[1]);
			break;

		case IND:
			sprintf(out + 4, "($%02X%02X)", bytes[2], bytes[1]);
			break;

		case ABSX:
			sprintf(out + 4, "$%02X%02X,x", bytes[2], bytes[1]);
			break;

		case ABSY:
			sprintf(out + 4, "$%02X%02X,y", bytes[2], bytes[1]);
			break;

		case REL:
			pc += (char) bytes[1] + 2;
			sprintf(out + 4, "$%02X%02X", (byte) (pc >> 8), (byte) pc);
			break;

		case INDEXX:
			sprintf(out + 4, "($%02X,x)", bytes[1]);
			break;

		case INDEXY:
			sprintf(out + 4, "($%02X),y", bytes[1]);
			break;

		case NONE:
		case UNKNOWN_ADDRESSING_MODE:
			break;
	}

	return len;
}
