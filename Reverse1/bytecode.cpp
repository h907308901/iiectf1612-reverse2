#define DEFINE_BC_FUNC
#include "all.h"

bool bc_execute(ByteCode* context) {
	__try {
		bc_table[*(context->ip++)](context);
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
	return true;
}

// bc_exit: exit execution
DEF_BC_FUNC(bc_exit) {
	return;
}

// bc_nop: do nothing
DEF_BC_FUNC(bc_nop) {
	BC_NEXT;
}

// bc_add: add reg2 to reg1
DEF_BC_FUNC(bc_add) {
	BC_GET_RR;
	context->reg[reg1] += context->reg[reg2];
	BC_NEXT;
}

// bc_sub: subtract reg2 from reg1
DEF_BC_FUNC(bc_sub) {
	BC_GET_RR;
	context->reg[reg1] -= context->reg[reg2];
	BC_NEXT;
}

// bc_mul: multiply reg1 by reg2
DEF_BC_FUNC(bc_mul) {
	BC_GET_RR;
	context->reg[reg1] *= context->reg[reg2];
	BC_NEXT;
}

// bc_div: divide reg1 by reg2
DEF_BC_FUNC(bc_div) {
	BC_GET_RR;
	context->reg[reg1] /= context->reg[reg2];
	BC_NEXT;
}

// bc_and: and
DEF_BC_FUNC(bc_and) {
	BC_GET_RR;
	context->reg[reg1] &= context->reg[reg2];
	BC_NEXT;
}

// bc_or: or
DEF_BC_FUNC(bc_or) {
	BC_GET_RR;
	context->reg[reg1] |= context->reg[reg2];
	BC_NEXT;
}

// bc_xor: exclusive or
DEF_BC_FUNC(bc_xor) {
	BC_GET_RR;
	context->reg[reg1] ^= context->reg[reg2];
	BC_NEXT;
}

// bc_mod: reg1 modulo reg2
DEF_BC_FUNC(bc_mod) {
	BC_GET_RR;
	context->reg[reg1] %= context->reg[reg2];
	BC_NEXT;
}

// bc_mov: let reg1 equal reg2
DEF_BC_FUNC(bc_mov) {
	BC_GET_RR;
	context->reg[reg1] = context->reg[reg2];
	BC_NEXT;
}

// bc_movi: let reg equal imm
DEF_BC_FUNC(bc_movi) {
	BC_GET_R0_IM;
	context->reg[reg] = imm;
	BC_NEXT;
}

// bc_inc: increase reg by 1
DEF_BC_FUNC(bc_inc) {
	BC_GET_R0;
	context->reg[reg]++;
	BC_NEXT;
}

// bc_dec: decrease reg by 1
DEF_BC_FUNC(bc_dec) {
	BC_GET_R0;
	context->reg[reg]--;
	BC_NEXT;
}

// bc_sys: call imm'th function in sysfunc table
DEF_BC_FUNC(bc_sys) {
	BC_GET_IM;
	if (context->sysfunc[imm])
		((BC_SYS_FUNC)context->sysfunc[imm])(context);
	BC_NEXT;
}

// bc_shl: shl in intel assembly
DEF_BC_FUNC(bc_shl) {
	BC_GET_R0_IM;
	context->reg[reg] <<= imm;
	BC_NEXT;
}

// bc_shr: shr in intel assembly
DEF_BC_FUNC(bc_shr){
	BC_GET_R0_IM;
	context->reg[reg] >>= imm;
	BC_NEXT;
}

// bc_rol: rol in intel assembly
DEF_BC_FUNC(bc_rol){
	BC_GET_R0_IM;
	imm %= (sizeof(void*) * 8);
	size_t val = context->reg[reg];
	context->reg[reg] = (val << imm) | (val >> (sizeof(void*) * 8 - imm));
	BC_NEXT;
}

// bc_ror: ror in intel assembly
DEF_BC_FUNC(bc_ror){
	BC_GET_R0_IM;
	imm %= (sizeof(void*) * 8);
	size_t val = context->reg[reg];
	context->reg[reg] = (val >> imm) | (val << (sizeof(void*) * 8 - imm));
	BC_NEXT;
}

// bc_cmp: compare reg1 and reg2
DEF_BC_FUNC(bc_cmp) {
	BC_GET_RR;
	if (context->reg[reg1] > context->reg[reg2])
		context->cmpresult = 1;
	else if(context->reg[reg1] == context->reg[reg2])
		context->cmpresult = 0;
	else if (context->reg[reg1] < context->reg[reg2])
		context->cmpresult = -1;
	BC_NEXT;
}

// bc_cmpi: compare reg and imm
DEF_BC_FUNC(bc_cmpi) {
	BC_GET_R0_IM;
	if (context->reg[reg] > imm)
		context->cmpresult = 1;
	else if(context->reg[reg] == imm)
		context->cmpresult = 0;
	else if (context->reg[reg] < imm)
		context->cmpresult = -1;
	BC_NEXT;
}

// bc_lodsb: load a byte to reg1 from where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_lodsb) {
	BC_GET_RR;
	context->reg[reg1] = *((unsigned char*)context->reg[reg2]);
	context->reg[reg2]++;
	BC_NEXT;
}

// bc_stosb: store a byte from reg1 to where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_stosb) {
	BC_GET_RR;
	*((unsigned char*)context->reg[reg2]) = (unsigned char)(context->reg[reg1] & 0xff);
	context->reg[reg2]++;
	BC_NEXT;
}

// bc_lodsw: load a word to reg1 from where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_lodsw) {
	BC_GET_RR;
	context->reg[reg1] = *((unsigned short*)context->reg[reg2]);
	context->reg[reg2] += 2;
	BC_NEXT;
}

// bc_stosw: store a word from reg1 to where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_stosw) {
	BC_GET_RR;
	*((unsigned short*)context->reg[reg2]) = (unsigned short)(context->reg[reg1] & 0xffff);
	context->reg[reg2] += 2;
	BC_NEXT;
}

// bc_lodsd: load a doubleword to reg1 from where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_lodsd) {
	BC_GET_RR;
	context->reg[reg1] = *((unsigned long*)context->reg[reg2]);
	context->reg[reg2] += 4;
	BC_NEXT;
}

// bc_stosd: store a doubleword from reg1 to where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_stosd) {
	BC_GET_RR;
	*((unsigned long*)context->reg[reg2]) = (unsigned long)(context->reg[reg1] & 0xffffffff);
	context->reg[reg2] += 4;
	BC_NEXT;
}

// bc_lodsq: load a quadword to reg1 from where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_lodsq) {
#ifdef _WIN64
	BC_GET_RR;
	context->reg[reg1] = *((unsigned long long*)context->reg[reg2]);
	context->reg[reg2] += 8;
	BC_NEXT;
#endif
}

// bc_stosq: store a quadword from reg1 to where reg2 asa a pointer indicates
DEF_BC_FUNC(bc_stosq) {
#ifdef _WIN64
	BC_GET_RR;
	*((unsigned long long*)context->reg[reg2]) = context->reg[reg1];
	context->reg[reg2] += 8;
	BC_NEXT;
#endif
}

// bc_jmp: jump by imm as an offset
DEF_BC_FUNC(bc_jmp) {
	BC_GET_IM;
	context->ip += imm;
	BC_NEXT;
}

// bc_jg: jump if greater
DEF_BC_FUNC(bc_jg) {
	BC_GET_IM;
	if (context->cmpresult > 0)
		context->ip += imm;
	BC_NEXT;
}

// bc_je: jump if equal
DEF_BC_FUNC(bc_je) {
	BC_GET_IM;
	if (context->cmpresult == 0)
		context->ip += imm;
	BC_NEXT;
}

// bc_jl: jump if less
DEF_BC_FUNC(bc_jl) {
	BC_GET_IM;
	if (context->cmpresult < 0)
		context->ip += imm;
	BC_NEXT;
}

// bc_jge: jump if greater or equal
DEF_BC_FUNC(bc_jge) {
	BC_GET_IM;
	if (context->cmpresult >= 0)
		context->ip += imm;
	BC_NEXT;
}

// bc_jle: jump if less or equal
DEF_BC_FUNC(bc_jle) {
	BC_GET_IM;
	if (context->cmpresult <= 0)
		context->ip += imm;
	BC_NEXT;
}

// bc_jne: jump if not equal
DEF_BC_FUNC(bc_jne) {
	BC_GET_IM;
	if (context->cmpresult != 0)
		context->ip += imm;
	BC_NEXT;
}
