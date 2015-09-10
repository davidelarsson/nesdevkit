#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "asm.h"
#include "cpu.h"

int opcodes[][12] = {
    /*  Ins |  Mode: NONE  IMM   ZP    ZPX   ZPY   ABS   ABSX  ABSY  REL   IND   INDEXX  INDEXY */
    /* ---------------------------------------------------------------------------------------- */
    /*  ADC */    {  -1,   0x69, 0x65, 0x75, -1   ,0x6D, 0x7D, 0x79, -1,   -1,   0x61,   0x71  },
    /*  AND */    {  -1,   0x29, 0x25, 0x35, -1,   0x2D, 0x3D, 0x39, -1,   -1,   0x21,   0x31  },
    /*  ASL */    {  0x0A, -1,   0x06, 0x16, -1,   0x0E, 0x1E, -1,   -1,   -1,   -1,     -1    },
    /*  BCC */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0x90,   -1,   -1,     -1    },
    /*  BCS */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0xB0,   -1,   -1,     -1    },
    /*  BEQ */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0xF0,   -1,   -1,     -1    },
    /*  BIT */    {  -1,   -1,   0x24, -1,   -1,   0x2C, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  BMI */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0x30,   -1,   -1,     -1    },
    /*  BNE */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0xD0,   -1,   -1,     -1    },
    /*  BPL */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0x10,   -1,   -1,     -1    },
    /*  BRK */    {  0x00, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  BVC */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0x50,   -1,   -1,     -1    },
    /*  BVS */    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0x70,   -1,   -1,     -1    },
    /*  CLC */    {  0x18, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  CLD */    {  0xD8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  CLI */    {  0x58, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  CLV */    {  0xB8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  CMP */    {  -1,   0xC9, 0xC5, 0xD5, -1,   0xCD, 0xDD, 0xD9, -1,   -1,   0xC1,   0xD1  },
    /*  CPX */    {  -1,   0xE0, 0xE4, -1,   -1,   0xEC, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  CPY */    {  -1,   0xC0, 0xC4, -1,   -1,   0xCC, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  DEC */    {  -1,   -1,   0x44, 0xD6, -1,   0xCE, 0xDE, -1,   -1,   -1,   -1,     -1    },
    /*  DEX */    {  0xCA, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  DEY */    {  0x88, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  EOR */    {  -1,   0x49, 0x45, 0x55, -1,   0x4D, 0x5D, 0x59, -1,   -1,   0x41,   0x51  },
    /*  INC */    {  -1,   -1,   0xE6, 0xF6, -1,   0xEE, 0xFE, -1,   -1,   -1,   -1,     -1    },
    /*  INX */    {  0xE8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  INY */    {  0xC8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  JMP */    {  -1,   -1,   -1,   -1,   -1,   0x4C, -1,   -1,   -1,   0x6C, -1,     -1    },
    /*  JSR */    {  -1,   -1,   -1,   -1,   -1,   0x20, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  LDA */    {  -1,   0xA9, 0xA5, 0xB5, -1,   0xAD, 0xBD, 0xB9, -1,   -1,   0xA1,   0xB1  },
    /*  LDX */    {  -1,   0xA2, 0xA6, -1,   0xB6, 0xAE, -1,   0xBE, -1,   -1,   -1,     -1    },
    /*  LDY */    {  -1,   0xA0, 0xA4, 0xB4, -1,   0xAC, 0xBC, -1,   -1,   -1,   -1,     -1    },
    /*  LSR */    {  -1,   0x4A, 0x46, 0x56, -1,   0x4E, 0x5E, -1,   -1,   -1,   -1,     -1    },
    /*  NOP */    {  0xEA, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  ORA */    {  -1,   0x09, 0x05, 0x15, -1,   0x0D, 0x1D, 0x19, -1,   -1,   0x01,   0x11  },
    /*  PHA */    {  0x48, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  PHP */    {  0x08, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  PLA */    {  0x68, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  PLP */    {  0x28, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  ROL */    {  0x2A, -1,   0x26, 0x36, -1,   0x2E, 0x3E, -1,   -1,   -1,   -1,     -1    },
    /*  ROR */    {  0x6A, -1,   0x66, 0x76, -1,   0x6E, 0x7E, -1,   -1,   -1,   -1,     -1    },
    /*  RTI */    {  0x40, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  RTS */    {  0x60, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  SBC */    {  -1,   0xE9, 0xE5, 0xF5, -1,   0xED, 0xFD, 0xF9, -1,   -1,   0xE1,   0xF1  },
    /*  SEC */    {  0x38, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  SED */    {  0xF8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  SEI */    {  0x78, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  STA */    {  -1,   -1,   0x85, 0x95, -1,   0x8D, 0x9D, 0x99, -1,   -1,   0x81,   0x91  },
    /*  STX */    {  -1,   -1,   0x86, -1,   0x96, 0x8E, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  STY */    {  -1,   -1,   0x84, 0x94, -1,   0x8C, -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TAX */    {  0xAA, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TAY */    {  0xA8, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TSX */    {  0xBA, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TXA */    {  0x8A, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TXS */    {  0x9A, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  TYA */    {  0x98, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /*  UNKNOWN_INSTRUCTION  */{-1,-1,-1,-1, -1,   -1,   -1,   -1,   -1,   -1,   -1,     -1    },
    /* ---------------------------------------------------------------------------------------- */
    /*  Ins |  Mode: NONE  IMM   ZP    ZPX   ZPY   ABS   ABSX  ABSY  REL   IND   INDEXX  INDEXY */
};


int nes_asm(char *str, word pc, byte *out)
{
	(void) pc;
	(void) out;
	unsigned start = 0;
	unsigned end = 0;
	unsigned index = 0;
	(void) index;
	char *workstr;

	out[0] = 0x00;
	out[1] = 0x00;
	out[2] = 0x00;

	printf("Assembling \"%s\"\n", str);

	/* Start beyond all preceding whitespaces */
	while(isspace(*(str + start)))
		start++;

	/* Find the first semicolon and if it exists, mark that as the end of the string */
	while(str[end] != ';' && end != strlen(str))
		end++;

	/* Remove all trailing whitespaces */
	while(start != end && isspace(str[end - 1]))
		end--;

	/* If the string is empty, we have nothing to assemble. Just exit and return zero */
	if(end == start)
		return 0;

	/* Let end point to the last character of the string */
	end--;

	/* Make sure the final string is at least three characters long */
	if((end + 1 - start) < 3)
	{
		printf("Instruction must be at least three letters\n");
		return -1;
	}
	
	unsigned workstr_len = end + 1 - start;
	workstr = malloc(sizeof(char) * workstr_len + 1); /* Allocate for NULL */
	strncpy(workstr, str + start, workstr_len);

	/* All uppercae. Makes comparsion easier */
	while(index < strlen(workstr))
	{
		workstr[index] = toupper(workstr[index]);
		index++;
	}

	instruction_t instr = parse_instruction(workstr);
	if(instr == UNKNOWN_INSTRUCTION)
	{
		printf("Could not parse intsruction!\n");
		return -1;
	}
	byte lower;
	byte upper;
	addressing_mode_t am = parse_addressing_mode(workstr, &lower, &upper);
	if(am == UNKNOWN_ADDRESSING_MODE)
	{
		printf("Could not parse addressing mode!\n");
		return -1;
	}

	/* Branch instructions with relative addressing mode need special treatment */
	if(instr == BCS || instr == BCC || instr == BPL || instr == BMI ||
		instr == BVC || instr == BVS || instr == BEQ || instr == BNE)
	{
		word arg = 0x0000;
		/* To the assembler it looks like it's either absolute or zero page mode */
		if(am == ZP)
		{
			arg = get_zp_byte(workstr);
		} else if(am == ABS)
		{
			arg = get_abs_byte_lo(workstr);
			arg |= (get_abs_byte_hi(workstr) << 8);
		} else
		{
			printf("Illegal combination of opcode and addressing mode!\n");
			return -1;
		}
		if(arg >= pc + 0x82)
		{
			printf("Relative address too high!\n");
			return -1;
		}
		if(arg <= pc - 0x7E)
		{
			printf("Relative address too low!\n");
			return -1;
		}
		/* But we know it's relative mode! */
		out[0] = opcodes[instr][REL];
		out[1] = arg - pc - 2;

		return 2;
	}

	int opcode = opcodes[instr][am];
	if(opcode < 0 || opcode > 255)
	{
		printf("Illegal combination of opcode and addressing mode!\n");
		return -1;
	}
	out[0] = opcode;
	out[1] = lower;
	out[2] = upper;
	return cpu_instruction_length[opcode];
}

instruction_t parse_instruction(char *str)
{
	if(!strncmp(str, "ADC", 3))
		return ADC;
	if(!strncmp(str, "AND", 3))
		return AND;
	if(!strncmp(str, "ASL", 3))
		return ASL;
	if(!strncmp(str, "BCC", 3))
		return BCC;
	if(!strncmp(str, "BCS", 3))
		return BCS;
	if(!strncmp(str, "BEQ", 3))
		return BEQ;
	if(!strncmp(str, "BIT", 3))
		return BIT;
	if(!strncmp(str, "BMI", 3))
		return BMI;
	if(!strncmp(str, "BNE", 3))
		return BNE;
	if(!strncmp(str, "BPL", 3))
		return BPL;
	if(!strncmp(str, "BRK", 3))
		return BRK;
	if(!strncmp(str, "BVC", 3))
		return BVC;
	if(!strncmp(str, "BVS", 3))
		return BVS;
	if(!strncmp(str, "CLC", 3))
		return CLC;
	if(!strncmp(str, "CLD", 3))
		return CLD;
	if(!strncmp(str, "CLI", 3))
		return CLI;
	if(!strncmp(str, "CLV", 3))
		return CLV;
	if(!strncmp(str, "CMP", 3))
		return CMP;
	if(!strncmp(str, "CPX", 3))
		return CPX;
	if(!strncmp(str, "CPY", 3))
		return CPY;
	if(!strncmp(str, "DEC", 3))
		return DEC;
	if(!strncmp(str, "DEX", 3))
		return DEX;
	if(!strncmp(str, "DEY", 3))
		return DEY;
	if(!strncmp(str, "EOR", 3))
		return EOR;
	if(!strncmp(str, "INC", 3))
		return INC;
	if(!strncmp(str, "INX", 3))
		return INX;
	if(!strncmp(str, "INY", 3))
		return INY;
	if(!strncmp(str, "JMP", 3))
		return JMP;
	if(!strncmp(str, "JSR", 3))
		return JSR;
	if(!strncmp(str, "LDA", 3))
		return LDA;
	if(!strncmp(str, "LDX", 3))
		return LDX;
	if(!strncmp(str, "LDY", 3))
		return LDY;
	if(!strncmp(str, "LSR", 3))
		return LSR;
	if(!strncmp(str, "NOP", 3))
		return NOP;
	if(!strncmp(str, "ORA", 3))
		return ORA;
	if(!strncmp(str, "PHA", 3))
		return PHA;
	if(!strncmp(str, "PHP", 3))
		return PHP;
	if(!strncmp(str, "PLA", 3))
		return PLA;
	if(!strncmp(str, "PLP", 3))
		return PHP;
	if(!strncmp(str, "ROL", 3))
		return ROL;
	if(!strncmp(str, "ROR", 3))
		return ROR;
	if(!strncmp(str, "RTI", 3))
		return RTI;
	if(!strncmp(str, "RTS", 3))
		return RTS;
	if(!strncmp(str, "SBC", 3))
		return SBC;
	if(!strncmp(str, "SEC", 3))
		return SEC;
	if(!strncmp(str, "SED", 3))
		return SED;
	if(!strncmp(str, "SEI", 3))
		return SEI;
	if(!strncmp(str, "STA", 3))
		return STA;
	if(!strncmp(str, "STX", 3))
		return STX;
	if(!strncmp(str, "STY", 3))
		return STY;
	if(!strncmp(str, "TAX", 3))
		return TAX;
	if(!strncmp(str, "TAY", 3))
		return TAY;
	if(!strncmp(str, "TSX", 3))
		return TSX;
	if(!strncmp(str, "TXA", 3))
		return TXA;
	if(!strncmp(str, "TXS", 3))
		return TXS;
	if(!strncmp(str, "TYA", 3))
		return TYA;
	return UNKNOWN_INSTRUCTION;
}

addressing_mode_t parse_addressing_mode(char *str, byte *lower_byte, byte *upper_byte)
{
	/* The argument starts after the three character instruction */
	unsigned start = 3;
	unsigned index = 0;

	while(isblank(str[start]) && start < strlen(str))
		start++;

	/* An argument that starts with a # is always immediate */
	if(str[start] == '#')
	{
		*lower_byte = get_imm_byte(str);
		return IMM;
	}

	/* An argument that starts with 'A' has accumulator as addressing mode.
	   We consider it to be none */
	if(str[start] == 'A')
	{
		/* Start checking for trailing characters after the 'A' */
		start++;
		while(strlen(str + start) > index)
		{
			if(!isblank(str[start + index]))
			{
				printf("Trailing characters: %s\n", str + start + index);
				return UNKNOWN_ADDRESSING_MODE;
			}
			index++;
		}

		return NONE;
	}

	/* An argument that starts with a left paranthesis is either index-x, index-y or 
	   indirect (only used by JMP) */
	if(str[start] == '(')
	{
		/* If the string does not contain ',' it is indirect addressing mode.
		   Otherwise it's either "(hh,x)" or "(hh),y". Determine by looking for x or y. */
		if(!strchr(str + start, ','))
		{

			*lower_byte = get_ind_byte_lo(str);
			*upper_byte = get_ind_byte_hi(str);
			return IND;
		}
		if(strchr(str + start, 'X'))
		{
			*lower_byte = get_indexx_byte(str);
			return INDEXX;
		}
		if(strchr(str + start, 'Y'))
		{
			*lower_byte = get_indexy_byte(str);
			return INDEXY;
		}
		return UNKNOWN_ADDRESSING_MODE;
	}

	/* An argument that starts with a '$' is either zero page, zero page-x, zero page-y,
	   absolute, absolute-x, or absolute-y */
	if(str[start] == '$')
	{
/* If the string does not contain a ',', it's either zero page or absolute addressnig mode */
		if(!strchr(str + start, ','))
		{
			/* How many digits? */
			start++;
			while(isxdigit(str[start + index]))
				index++;
			if(index < 1)
				return UNKNOWN_ADDRESSING_MODE;
			if(index < 3)
			{
				*lower_byte = get_zp_byte(str);
				return ZP;
			}
			if(index < 5)
			{
				*lower_byte = get_abs_byte_lo(str);
				*upper_byte = get_abs_byte_hi(str);
				return ABS;
			}
			else
				return UNKNOWN_ADDRESSING_MODE;
		}
		/* We have either zp,x; zp,y; abs,x; or abs,y */
		start++;
		while(isxdigit(str[start + index]))
			index++;
		if(index < 1)
			return UNKNOWN_ADDRESSING_MODE;
		if(index < 3)
			/* Either zp,x or zp,y */
		{
			if(strchr(str + start, 'X'))
			{
				*lower_byte = get_zpx_byte(str);
				return ZPX;
			}
			if
			(strchr(str + start, 'Y'))
			{
				*lower_byte = get_zpy_byte(str);
				return ZPY;
			}
		}
		if(index < 5)
			/* Either abs,x or abs,y */
		{
			if(strchr(str + start, 'X'))
			{
				*lower_byte = get_absx_byte_lo(str);
				*upper_byte = get_absx_byte_hi(str);
				return ABSX;
			}
			if(strchr(str + start, 'Y'))
			{
				*lower_byte = get_absy_byte_lo(str);
				*upper_byte = get_absy_byte_hi(str);
				return ABSY;
			}
		}

		return UNKNOWN_ADDRESSING_MODE;
	}


	return UNKNOWN_ADDRESSING_MODE;
}

int get_imm_byte(char *str)
{
	unsigned start = 3;
	unsigned index = 0;

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next characters are '#$' */
	if(str[start] != '#')
	{
		printf("Syntax error: Immediate mode must start with a '#'.\n");
		return -1;
	}
	start++;
	if(str[start] != '$')
	{
		printf("Syntax error: Number must start with a '$'.\n");
		return -1;
	}

	/* The rest of the string must be one or two hexadecimal digits */
	start++;
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Immediate addressing mode must use a one-byte argument!\n");
		return -1;
	}
	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}

	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

int get_zp_byte(char *str)
{
	unsigned start = 3;
	unsigned index = 0;

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: Zero page mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The rest of the string must be one to two hexadecimal digits */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Zero page addressing mode must use a one-byte argument!\n");
		return -1;
	}
	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

int get_zpx_byte(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: Zero page,x mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next one to two digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Zero page,x addressing mode must use a one-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Zero page,x addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an X */
	if(str[start + index] != 'X')
	{
		printf("Zero page,x addressing mode must contain an X!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

int get_zpy_byte(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: Zero page,y mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next one to two digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Zero page,y addressing mode must use a one-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Zero page,y addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an Y */
	if(str[start + index] != 'Y')
	{
		printf("Zero page,y addressing mode must contain a Y!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

int get_abs_byte_hi(char *str)
{
	unsigned start = 3;
	unsigned index = 0;

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: Absolute addressing mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The rest of the string must be three to four hexadecimal digits */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute addressing mode must use a two-byte argument!\n");
		return -1;
	}

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	val >>= 8;
	return val;
}

int get_abs_byte_lo(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */
	unsigned end = 0; 	/* End of digits to be converted to a value */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: Absolute addressing mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The rest of the string must be three to four hexadecimal digits */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute addressing mode must use a two-byte argument!\n");
		return -1;
	}
	/* If syntax is correct, this is the end of the digits */
	end = index;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	/* Byte two starts one or two digits after the first */
	sscanf(str + start + ((end == 3) ? 1 : 2), "%x", &val); 
	val &= 0xFF;
	return val;
}

int get_ind_byte_hi(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '(' */
	if(str[start] != '(')
	{
		printf("Syntax error: indirect mode must start with a '('.\n");
		return -1;
	}
	start++;

	/* Move beyond whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: indirect mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Indirect addressing mode must use a two-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a ')' */
	if(str[start + index] != ')')
	{
		printf("Indirect addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	val >>= 8;
	return val;
}

int get_ind_byte_lo(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */
	unsigned end = 0;	/* End of digit string. Not needed for this function since the
						   end of the digit string is also the end of the string, but
						   the other addressing modes have it, and I like uniformity! */


	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '(' */
	if(str[start] != '(')
	{
		printf("Syntax error: indirect mode must start with a '('.\n");
		return -1;
	}
	start++;

	/* Move beyond whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: indirect mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Indirect addressing mode must use a two-byte argument!\n");
		return -1;
	}
	/* If syntax is correct, this is the end of the digits */
	end = index;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a ')' */
	if(str[start + index] != ')')
	{
		printf("Indirect addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	/* Byte two starts one or two digits after the first */
	sscanf(str + start + ((end == 3) ? 1 : 2), "%x", &val); 
	return val;
}

int get_absx_byte_hi(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: absolute,x mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute,x addressing mode must use a two-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Absolute,x addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an X */
	if(str[start + index] != 'X')
	{
		printf("Absolute,x addressing mode must contain an X!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	val >>= 8;
	return val;
}

int get_absx_byte_lo(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */
	unsigned end = 0; 	/* End of digits to be converted to a value */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: absolute,x mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute,x addressing mode must use a two-byte argument!\n");
		return -1;
	}
	/* If syntax is correct, this is the end of the digits */
	end = index;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Absolute,x addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an X */
	if(str[start + index] != 'X')
	{
		printf("Absolute,x addressing mode must contain an X!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	/* Byte two starts one or two digits after the first */
	sscanf(str + start + ((end == 3) ? 1 : 2), "%x", &val);
	return val;
}

int get_absy_byte_hi(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: absolute,y mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute,y addressing mode must use a two-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Absolute,y addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an Y */
	if(str[start + index] != 'Y')
	{
		printf("Absolute,y addressing mode must contain a Y!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	val >>= 8;
	return val;
}

int get_absy_byte_lo(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */
	unsigned end = 0;	/* End of digits */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the next character is a '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: absolute,y mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next three to four digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 3 || index > 4)
	{
		printf("Absolute,y addressing mode must use a two-byte argument!\n");
		return -1;
	}
	/* If syntax is correct, this is the end of the digits */
	end = index;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Absolute,y addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an Y */
	if(str[start + index] != 'Y')
	{
		printf("Absolute,y addressing mode must contain an Y!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	/* Byte two starts one or two digits after the first */
	sscanf(str + start + ((end == 3) ? 1 : 2), "%x", &val);
	return val;
}

int get_indexx_byte(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the first character is '(' */
	if(str[start] != '(')
	{
		printf("Syntax error: index-x mode must start with a '('.\n");
		return -1;
	}
	start++;

	/* Move past possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* The next character must be '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: index-x mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next one to two digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Index-x addressing mode must use a one-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a comma */
	if(str[start + index] != ',')
	{
		printf("Index-x addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an X */
	if(str[start + index] != 'X')
	{
		printf("Index-x addressing mode must contain an X!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a ')' */
	if(str[start + index] != ')')
	{
		printf("Index-x addressing mode must contain an X!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

int get_indexy_byte(char *str)
{
	unsigned start = 3; /* Start of the acutal argument (after the $ sign) */
	unsigned index = 0; /* Counter */

	/* Move past the instruction and possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* Make sure the first character is '(' */
	if(str[start] != '(')
	{
		printf("Syntax error: index-y mode must start with a '('.\n");
		return -1;
	}
	start++;

	/* Move past possible whitespaces */
	while(isspace(str[start]))
		start++;

	/* The next character must be '$' */
	if(str[start] != '$')
	{
		printf("Syntax error: index-y mode must start with a '$'.\n");
		return -1;
	}
	start++;

	/* The next one to two digits must be a hexadecimal value */
	while(isxdigit(str[start + index]))
		index++;
	if(index < 1 || index > 2)
	{
		printf("Index-y addressing mode must use a one-byte argument!\n");
		return -1;
	}

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a ')' */
	if(str[start + index] != ')')
	{
		printf("Index-y addressing mode must contain a ')'!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be an ',' */
	if(str[start + index] != ',')
	{
		printf("Index-y addressing mode must contain a comma!\n");
		return -1;
	}
	index++;

	/* Move beyond whitespaces */
	while(isspace(str[start + index]))
		index++;

	/* Here must be a Y */
	if(str[start + index] != 'Y')
	{
		printf("Index-y addressing mode must contain a Y!\n");
		return -1;
	}
	index++;

	/* Is that all? We want no trailing characters! */
	if(strlen(str) != start + index)
	{
		printf("Syntax error: Trailing characters!\n");
		return -1;
	}
	int val;
	sscanf(str + start, "%x", &val);
	return val;
}

instruction_t parse_instruction_type(const char *name) {

    if (!name)
        return UNKNOWN_INSTRUCTION;

    if (!strncmp(name, "ADC", 3))
        return ADC;
    else if (!strncmp(name, "AND", 3))
        return AND;
    else if (!strncmp(name, "ASL", 3))
        return ASL;
    /* Osv osv */
    else
        return UNKNOWN_INSTRUCTION;
}

