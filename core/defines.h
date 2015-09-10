#ifndef DEFINES_H
#define DEFINES_H

#define byte unsigned char
#define word unsigned short
#define NMI_VECTOR 0xFFFE
#define STACK_OFFSET 0x0100
//#define address word

#define CPU_RAM_SIZE 0x800
#define PPU_RAM_SIZE 0x4000
#define OAM_RAM_SIZE 64 * 4
#define SPRITE_RAM_SIZE 0x100
#define PALETTE_RAM_SIZE 0x20

typedef enum {
	VERTICAL_MIRRORING,
	HORIZONTAL_MIRRORING,
	FOUR_SCREEN
} mirroring_t;

/* Note that the order and content of this enum is important and must not be changed! */
typedef enum {
	NONE,	/* No (or implied) addressing mode */
	IMM,	/* Immediate addressing mode */
	ZP,		/* Zero page addressing mode */
	ZPX,	/* Zero page,x addressing mode */
	ZPY,	/* Zero page,y addressing mode */
	ABS,	/* Absolute addressing mode */
	ABSX,	/* Absolute,x addressing mode */
	ABSY,	/* Absolute,y addressing mode */
	REL,	/* Relative addressing mode */
	IND,	/* Indirect addressing mode */
	INDEXX,	/* (index,x) addressign mode */
	INDEXY,	/* (index),y addressing mode */
	UNKNOWN_ADDRESSING_MODE
} addressing_mode_t;

/* Note that the order and content of this enum is important and must not be changed! */
typedef enum {
    ADC, /* add with carry */
    AND, /* and (with accumulator) */
    ASL, /* arithmetic shift left */
    BCC, /* branch on carry clear */
    BCS, /* branch on carry set */
    BEQ, /* branch on equal (zero set) */
    BIT, /* bit test */
    BMI, /* branch on minus (negative set) */
    BNE, /* branch on not equal (zero clear) */
    BPL, /* branch on plus (negative clear) */
    BRK, /* interrupt */
    BVC, /* branch on overflow clear */
    BVS, /* branch on overflow set */
    CLC, /* clear carry */
    CLD, /* clear decimal */
    CLI, /* clear interrupt disable */
    CLV, /* clear overflow */
    CMP, /* compare (with accumulator) */
    CPX, /* compare with X */
    CPY, /* compare with Y */
    DEC, /* decrement */
    DEX, /* decrement X */
    DEY, /* decrement Y */
    EOR, /* exclusive or (with accumulator) */
    INC, /* increment */
    INX, /* increment X */
    INY, /* increment Y */
    JMP, /* jump */
    JSR, /* jump subroutine */
    LDA, /* load accumulator */
    LDX, /* load X */
    LDY, /* load Y */
    LSR, /* logical shift right */
    NOP, /* no operation */
    ORA, /* or with accumulator */
    PHA, /* push accumulator */
    PHP, /* push processor status (SR) */
    PLA, /* pull accumulator */
    PLP, /* pull processor status (SR) */
    ROL, /* rotate left */
    ROR, /* rotate right */
    RTI, /* return from interrupt */
    RTS, /* return from subroutine */
    SBC, /* subtract with carry */
    SEC, /* set carry */
    SED, /* set decimal */
    SEI, /* set interrupt disable */
    STA, /* store accumulator */
    STX, /* store X */
    STY, /* store Y */
    TAX, /* transfer accumulator to X */
    TAY, /* transfer accumulator to Y */
    TSX, /* transfer stack pointer to X */
    TXA, /* transfer X to accumulator */
    TXS, /* transfer X to stack pointer */
    TYA, /* transfer Y to accumulator */

    UNKNOWN_INSTRUCTION
} instruction_t;

#endif /* DEFINES_H */
