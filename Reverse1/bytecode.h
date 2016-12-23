#pragma once
#include "all.h"

struct ByteCode {
	const char* ip = 0; // instruction pointer
	LONG_PTR cmpresult; // comparison result (1:gt 0:eq -1:lt)
	size_t reg[16] = { 0 }; // 16 registers (R0~R15)
	void* sysfunc[256] = { 0 }; // function table for bc_sys
};

// Execute the byte code
// Before calling this, ip and sysfunc must be set correctly
bool bc_execute(ByteCode* context);

// All functions to be called by bc_sys must be consistent with this
typedef void(*BC_SYS_FUNC)(ByteCode* context);

// These definitions are for byte code assembly
// For the meaning of each instruction see bytecode.cpp

#define R0	0
#define R1	1
#define R2	2
#define R3	3
#define R4	4
#define R5	5
#define R6	6
#define R7	7
#define R8	8
#define R9	9
#define R10	10
#define R11	11
#define R12	12
#define R13	13
#define R14	14
#define R15	15

#define BC_ASM_BEGIN(x)			static const char x[] = {
#define BC_ASM_END				0x00 };

#define BC_EXIT					0x00,
#define BC_NOP					0x01,
#define BC_ADD(r1, r2)			0x02, r1 << 4 | r2,
#define BC_SUB(r1, r2)			0x03, r1 << 4 | r2,
#define BC_MUL(r1, r2)			0x04, r1 << 4 | r2,
#define BC_DIV(r1, r2)			0x05, r1 << 4 | r2,
#define BC_AND(r1, r2)			0x06, r1 << 4 | r2,
#define BC_OR(r1, r2)			0x07, r1 << 4 | r2,
#define BC_XOR(r1, r2)			0x08, r1 << 4 | r2,
#define BC_MOD(r1, r2)			0x09, r1 << 4 | r2,
#define BC_MOV(r1, r2)			0x0A, r1 << 4 | r2,
#define BC_MOVI(r, i)			0x0B, r << 4, i,
#define BC_INC(r)				0x0C, r << 4,
#define BC_DEC(r)				0x0D, r << 4,

#define BC_SYS(i)				0x0F, i,

#define BC_SHL(r, i)			0x10, r << 4, i,
#define BC_SHR(r, i)			0x11, r << 4, i,
#define BC_ROL(r, i)			0x12, r << 4, i,
#define BC_ROR(r, i)			0x13, r << 4, i,
#define BC_CMP(r1, r2)			0x14, r1 << 4 | r2,
#define BC_CMPI(r, i)			0x15, r << 4, i,
#define BC_LODI(r, i)			0x16, r << 4, i,
#define BC_STOI(r, i)			0x17, r << 4, i,
#define BC_LODSB(r1, r2)		0x18, r1 << 4 | r2,
#define BC_STOSB(r1, r2)		0x19, r1 << 4 | r2,
#define BC_LODSW(r1, r2)		0x1A, r1 << 4 | r2,
#define BC_STOSW(r1, r2)		0x1B, r1 << 4 | r2,
#define BC_LODSD(r1, r2)		0x1C, r1 << 4 | r2,
#define BC_STOSD(r1, r2)		0x1D, r1 << 4 | r2,
#ifdef _WIN64
#define BC_LODSQ(r1, r2)		0x1E, r1 << 4 | r2,
#define BC_STOSQ(r1, r2)		0x1F, r1 << 4 | r2,
#endif

#define BC_JMP(i)				0x20, i
#define BC_JG(i)				0x21, i,
#define BC_JE(i)				0x22, i,
#define BC_JL(i)				0x23, i,
#define BC_JGE(i)				0x24, i,
#define BC_JLE(i)				0x25, i,
#define BC_JNE(i)				0x26, i,

// These are only defined in bytecode.cpp
// The reason why I put them here is for the convenience of modification

#ifdef DEFINE_BC_FUNC

typedef void(*BC_FUNC)(ByteCode* context); // All interpreter functions must be consistent with this

#define DEF_BC_FUNC(x) void x(ByteCode* context)

// RR: this byte indicates reg1(hi 8-bit) and reg2(lo 8-bit)
// R0: tihs byte indicates reg(hi 8-bit)
// IM: this byte indicates imm(only -128~127)

// The first byte is opcode

DEF_BC_FUNC(bc_exit);	// 00
DEF_BC_FUNC(bc_nop);	// 01
DEF_BC_FUNC(bc_add);	// 02 RR
DEF_BC_FUNC(bc_sub);	// 03 RR
DEF_BC_FUNC(bc_mul);	// 04 RR
DEF_BC_FUNC(bc_div);	// 05 RR
DEF_BC_FUNC(bc_and);	// 06 RR
DEF_BC_FUNC(bc_or);		// 07 RR
DEF_BC_FUNC(bc_xor);	// 08 RR
DEF_BC_FUNC(bc_mod);	// O9 RR
DEF_BC_FUNC(bc_mov);	// 0A RR
DEF_BC_FUNC(bc_movi);	// 0B R0 IM
DEF_BC_FUNC(bc_inc);	// 0C R0
DEF_BC_FUNC(bc_dec);	// 0D R0

DEF_BC_FUNC(bc_sys);	// 0F IM

DEF_BC_FUNC(bc_shl);	// 10 R0 IM
DEF_BC_FUNC(bc_shr);	// 11 R0 IM
DEF_BC_FUNC(bc_rol);	// 12 R0 IM
DEF_BC_FUNC(bc_ror);	// 13 R0 IM
DEF_BC_FUNC(bc_cmp);	// 14 RR
DEF_BC_FUNC(bc_cmpi);	// 15 R0 IM
//DEF_BC_FUNC(bc_lodi);	// 16 R0 IM
//DEF_BC_FUNC(bc_stoi);	// 17 R0 IM
DEF_BC_FUNC(bc_lodsb);	// 18 RR
DEF_BC_FUNC(bc_stosb);	// 19 RR
DEF_BC_FUNC(bc_lodsw);	// 1A RR
DEF_BC_FUNC(bc_stosw);	// 1B RR
DEF_BC_FUNC(bc_lodsd);	// 1C RR
DEF_BC_FUNC(bc_stosd);	// 1D RR
DEF_BC_FUNC(bc_lodsq);	// 1E RR
DEF_BC_FUNC(bc_stosq);	// 1F RR

DEF_BC_FUNC(bc_jmp);	// 20 IM
DEF_BC_FUNC(bc_jg);		// 21 IM
DEF_BC_FUNC(bc_je);		// 22 IM
DEF_BC_FUNC(bc_jl);		// 23 IM
DEF_BC_FUNC(bc_jge);	// 24 IM
DEF_BC_FUNC(bc_jle);	// 25 IM
DEF_BC_FUNC(bc_jne);	// 26 IM

DEF_BC_FUNC(bc_dummy) {
	return;
}

static const BC_FUNC bc_table[256] = {
	// 00 - 0F
	bc_exit,	bc_nop,		bc_add,		bc_sub,
	bc_mul,		bc_div,		bc_and,		bc_or,
	bc_xor,		bc_mod,		bc_mov,		bc_movi,
	bc_inc,		bc_dec,		bc_dummy,	bc_sys,
	// 10 - 1F
	bc_shl,		bc_shr,		bc_rol,		bc_ror,
	bc_cmp,		bc_cmpi,	bc_dummy,	bc_dummy,
	bc_lodsb,	bc_stosb,	bc_lodsw,	bc_stosw,
	bc_lodsd,	bc_stosd,	bc_lodsq,	bc_stosq,
	// 20 - 2F
	bc_jmp,		bc_jg,		bc_je,		bc_jl,
	bc_jge,		bc_jle,		bc_jne
};

#define BC_NEXT BC_FUNC bc_next_func = bc_table[*(context->ip++)]; bc_next_func(context);
#define BC_GET_R0 char reg; reg = *(context->ip++); reg = reg >> 4;
#define BC_GET_RR char reg1, reg2, rr; rr = *(context->ip++); reg1 = rr >> 4; reg2 = rr & '\xf';
#define BC_GET_IM char imm; imm = *(context->ip++); 
#define BC_GET_R0_IM char reg, imm; reg = *(context->ip++); reg = reg >> 4; imm = *(context->ip++);

#endif