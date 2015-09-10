#ifndef CPU_H
#define CPU_H

#define MASK_CARRY 0x01
#define MASK_ZERO 0x02
#define MASK_INTERRUPT 0x04
#define MASK_DECIMAL 0x08
#define MASK_BREAK 0x10
//#define MASK_ALWAYS 0x20
#define MASK_OVERFLOW 0x40
#define MASK_NEGATIVE 0x80

#include <stdbool.h>
#include "defines.h"

typedef struct cpu_s {
	byte a; /* Accumulator */
	byte x; /* X Index register */
	byte y; /* Y Index register */
	word pc; /* Program Counter */
	byte sp; /* Stack Pointer */
	byte p; /* Status Register, i.e. flags */
	unsigned long long cycles; /* Number of cycles executed */
} cpu_t;

/**
 * Array of cycles consumed per instruction
 */
byte cpu_cycles[256];

/**
 * Array of length of instructions
 */
byte cpu_instruction_length[256];

/** Initialize the CPU */
cpu_t *cpu_init();

/** Initializes the cycle table */
void cpu_cycles_init();

/** Initializes the instruction length table */
void cpu_instruction_length_init();

/** Destroy the CPU */
void cpu_destroy(cpu_t *cpu);

/** Add clock cycles to the counter */
void cpu_add_cycles(cpu_t *cpu, unsigned cycles);

/** Get the number of consumed cycled */
unsigned long long cpu_get_cycles(cpu_t *cpu);

/** Get accumulator */
byte cpu_get_a(cpu_t *cpu);

/** Get X register */
byte cpu_get_x(cpu_t *cpu);

/** Get Y register */
byte cpu_get_y(cpu_t *cpu);

/** Get the stack pointer */
byte cpu_get_sp(cpu_t *cpu);

/** Get the flag register */
byte cpu_get_p(cpu_t *cpu);

/** Get the program counter */
word cpu_get_pc(cpu_t *cpu);

/** Set accumulator */
void cpu_set_a(cpu_t *cpu, byte val);

/** Set the X register */
void cpu_set_x(cpu_t *cpu, byte val);

/** Set the Y register */
void cpu_set_y(cpu_t *cpu, byte val);

/** Set the stack pointer */
void cpu_set_sp(cpu_t *cpu, byte address);

/** Set the flag register */
void cpu_set_p(cpu_t *cpu, byte val);

/** Set the program counter */
void cpu_set_pc(cpu_t *cpu, word address);

/** Set carry flag */
void cpu_set_c(cpu_t *cpu);

/** Set zero flag */
void cpu_set_z(cpu_t *cpu);

/** Set interrupt flag */
void cpu_set_i(cpu_t *cpu);

/** Set decimal flag */
void cpu_set_d(cpu_t *cpu);

/** Set break flag */
void cpu_set_b(cpu_t *cpu);

/** Set overflow flag */
void cpu_set_v(cpu_t *cpu);

/** Set negative flag */
void cpu_set_n(cpu_t *cpu);

/** Clear carry flag */
void cpu_clear_c(cpu_t *cpu);

/** Clear zero flag */
void cpu_clear_z(cpu_t *cpu);

/** Clear interrupt flag */
void cpu_clear_i(cpu_t *cpu);

/** Clear decimal flag */
void cpu_clear_d(cpu_t *cpu);

/** Clear break flag */
void cpu_clear_b(cpu_t *cpu);

/** Clear overflow flag */
void cpu_clear_v(cpu_t *cpu);

/** Clear negative flag */
void cpu_clear_n(cpu_t *cpu);

/** Get carry flag */
bool cpu_get_c(cpu_t *cpu);

/** Get zero flag */
bool cpu_get_z(cpu_t *cpu);

/** Get interrupt flag */
bool cpu_get_i(cpu_t *cpu);

/** Get decimal flag */
bool cpu_get_d(cpu_t *cpu);

/** Get break flag */
bool cpu_get_b(cpu_t *cpu);

/** Get break flag */
bool cpu_get_b(cpu_t *cpu);

/** Get overflow flag */
bool cpu_get_v(cpu_t *cpu);

/** Get negative flag */
bool cpu_get_n(cpu_t *cpu);



#endif /* CPU_H */
