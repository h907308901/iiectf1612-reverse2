#include "all.h"

// This program is successfully compiled using configuration Release x64.
// Other configurations are not guaranteed.
// Configuration for Release x64 is set correctly. Be careful when changing the settings.

ByteCode bc;
TlsPtr* tp; 
// buf: input buffer
char buf[32];
// str: encrypted flag string
static const char str[] = "\xa6\x83\x6e\x77\xb2\x89\x77\xa2\x85\x26\x6c\x79\x96\x92\x69\x6f\xa1\x83\x7d\x88\x69\x89\x78\x90\x88\x75\x85\x78";
// codeexit: some bc_exit code to let ByteCode interpreter exit
static const char codeexit[] = "\0\0\0";

void main();

#ifndef DEBUG_BUILD

// This SEH filter is used for anti-debug
// I do not guarantee this function works exactly correctly
DWORD ExceptionFilter(DWORD ExceptionCode, PVOID ExceptionInformation) {
	CONTEXT* ctxt = ((EXCEPTION_POINTERS*)ExceptionInformation)->ContextRecord;
	switch (ExceptionCode) {
	case EXCEPTION_BREAKPOINT: // int3 or something
		FixImage();
		break;
	case EXCEPTION_SINGLE_STEP: // single step
		ctxt->EFlags ^= 1 << 8;
		break;
	default:
		return EXCEPTION_CONTINUE_SEARCH;
	}
	ctxt->Dr0 = 0;
	ctxt->Dr1 = 0;
	ctxt->Dr2 = 0;
	ctxt->Dr3 = 0;
	ctxt->Dr6 = 0;
	ctxt->Dr7 = 0;
	return EXCEPTION_CONTINUE_EXECUTION;
}

#endif

// 0th function used by bc_sys
// Get flag input
void sysfunc00(ByteCode* context) {
	printf("Input your flag:"); // flag{AntiDbg&ByteCodeAREfun} 28 bytes
	gets_s(buf);
	if (lstrlenA(buf) != 28) {
		printf("Length mismatch\n");
		context->ip = codeexit;
	}
	context->reg[13] = (size_t)str;
	context->reg[14] = (size_t)buf;
	context->reg[15] = (size_t)buf;
}

// 1st function used by bc_sys
// Simply shows "Correct"
void sysfunc01(ByteCode* context) {
	printf("Correct\n");
}

// 2nd function used by bc_sys
// Simply shows "Incorrect"
void sysfunc02(ByteCode* context) {
	printf("Incorrect\n");
}

// 3rd function used by bc_sys
// Exchange r0'th and r1'th of input buffer
void sysfunc03(ByteCode* context) {
	char tmp;
	tmp = buf[context->reg[0]];
	buf[context->reg[0]] = buf[context->reg[1]];
	buf[context->reg[1]] = tmp;
}

// Real main routine
void inline main_real() {
#ifndef DEBUG_BUILD
	if (IsDebuggerPresent()) return; // a very easy anti-debug
	tp = GetApiAddresses(); // see myapi.cpp
	DebuggerCheck(tp); // see antidbg.cpp
	PatchBreakin(); // see antidbg.cpp
#endif
	// These are some byte codes I defined
	// For definitions see bytecode.h
	// For implementations see bytecode.cpp
	BC_ASM_BEGIN(code)
	BC_SYS(0)
	BC_MOVI(R0, 0)
	BC_MOVI(R2, 3)
	BC_MOVI(R3, 28)
	// LOC1
	BC_MOV(R1, R0)
	BC_MUL(R1, R2)
	BC_INC(R1)
	BC_MOD(R1, R3)
	BC_SYS(3)
	BC_INC(R0)
	BC_CMP(R0, R3)
	BC_JL(-16) // LOC1
	BC_MOVI(R1, 53)
	BC_MOVI(R2, 0)
	BC_MOVI(R4, 16)
	BC_MUL(R4, R4)
	BC_DEC(R4)
	// LOC2
	BC_LODSB(R0, R14)
	BC_XOR(R0, R1)
	BC_ADD(R0, R1)
	BC_AND(R0, R4)
	BC_STOSB(R0, R15)
	BC_LODSB(R1, R13)
	BC_CMP(R0, R1)
	BC_JE(3)
	BC_SYS(2)
	BC_EXIT
	BC_INC(R1)
	BC_INC(R2)
	BC_CMP(R2, R3)
	BC_JL(-27)// LOC2
	BC_SYS(1)
	BC_ASM_END
	bc.ip = code;
	bc.sysfunc[0] = sysfunc00;
	bc.sysfunc[1] = sysfunc01;
	bc.sysfunc[2] = sysfunc02;
	bc.sysfunc[3] = sysfunc03;
	bc_execute(&bc);
}

// This is a wrapper function
void main() {
#ifdef DEBUG_BUILD
	main_real();
#else
	__try {
		main_real();
	}
	__except (ExceptionFilter(GetExceptionCode(), GetExceptionInformation())) {

	}
#endif
}