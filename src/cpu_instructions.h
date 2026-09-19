#pragma once
#include <stdint.h>

struct gb_cpu_instruction {
	const char* disassembly;
	uint8_t operand_length;
	void* execute;
};

struct gb_cpu_pre_cb_instruction {
	const char* disassembly;
	void* execute;
};

extern const gb_cpu_instruction instructions[256];
extern const gb_cpu_pre_cb_instruction cb_instructions[256];