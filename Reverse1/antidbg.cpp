#include "all.h"

#ifndef DEBUG_BUILD

PVOID BaseAddress = NULL; // the base address of the image of the program

void CheckSum();

// These are used to define a TLS callback function
#ifdef _M_IX86
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")
#else
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback")
#endif

// TLS callback function will be executed before it comes to entry point
void NTAPI __stdcall TLS_CALLBACK(PVOID DllHandle, DWORD Reason, PVOID Reserved) {
	CheckSum();
	FixImage();
}

EXTERN_C
#ifdef _M_X64
#pragma const_seg (".CRT$XLB")
const
#else
#pragma data_seg (".CRT$XLB")
#endif

PIMAGE_TLS_CALLBACK _tls_callback[] = { TLS_CALLBACK,  0 };
#pragma data_seg ()
#pragma const_seg ()

// We hook DbgUiRemoteBreakin to this function
// DbgUiRemoteBreakin is called when a debugger has attached the program
void HookFunc() {
	// Do not use ExitProcess, or it crashes in LdrShutdownProcess due to stack error
	TerminateProcess((HANDLE)-1, (UINT)-1);
}

// This function just removes the debugger and this program continues execution
void DebuggerCheck(TlsPtr* tp) {
	HANDLE hDbgObj;
	NTQUERYINFORMATIONPROCESS NtQueryInformationProcess = (NTQUERYINFORMATIONPROCESS)tp->Fetch(API_QUERYINFORMATIONPROCESS);
	NTREMOVEPROCESSDEBUG NtRemoveProcessDebug = (NTREMOVEPROCESSDEBUG)tp->Fetch(API_REMOVEPROCESSDEBUG);
	NtQueryInformationProcess((HANDLE)-1, 0x0000001e, &hDbgObj, sizeof(hDbgObj), NULL);
	NtRemoveProcessDebug((HANDLE)-1, hDbgObj);
}

// This function load the image file of this program and calculate its checksum to determine if it's tampered
// Also the loaded image is used by FixImage
void CheckSum() {
	HANDLE hFile, hFileMapping;
	wchar_t path[512];
	int size;
	GetModuleFileName(NULL, path, 511);
	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		size = GetFileSize(hFile, NULL);
		hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		CloseHandle(hFile);
		if (hFileMapping) {
			BaseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
			CloseHandle(hFileMapping);
			if (BaseAddress) {
				DWORD checksum1, checksum2;
				CheckSumMappedFile(BaseAddress, size, &checksum1, &checksum2);
				//printf("CheckSum: 0x%08X, 0x%08X\n", checksum1, checksum2);
				if (checksum1 == checksum2) return;
				UnmapViewOfFile(BaseAddress);
			}
			else {
				//printf("MapViewOfFile failed %d\n", GetLastError());
			}
		}
		else {
			//printf("CreateFileMapping failed %d\n", GetLastError());
		}
	}
	else {
		//printf("CreateFile failed %d\n", GetLastError());
	}
	FatalAppExitA(0, "Checksum mismatch");
}

// This function fixes the program if there are breakpoints or something
void FixImage() {
	IMAGE_DOS_HEADER *dosheader;
	IMAGE_NT_HEADERS *ntheader;
	IMAGE_SECTION_HEADER *sec;
	MEMORY_BASIC_INFORMATION mbi;
	if (!BaseAddress)
		FatalAppExitA(0, "Uninitialized pointer");
	dosheader = (IMAGE_DOS_HEADER*)BaseAddress;
	ntheader = (IMAGE_NT_HEADERS*)((ULONG_PTR)dosheader + dosheader->e_lfanew);
	sec = IMAGE_FIRST_SECTION(ntheader);
	VirtualQuery(FixImage, &mbi, sizeof(mbi));
	WriteProcessMemory(
		(HANDLE)-1,
		mbi.BaseAddress,
		(PVOID)((ULONG_PTR)dosheader + sec->PointerToRawData),
		sec->Misc.VirtualSize,
		NULL
	);
}

// This function hooks DbgUiRemoteBreakin to HookFunc
void PatchBreakin() {
#ifdef _WIN64
	void* func = HookFunc;
	unsigned char asmcode[] = { 0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0 };
	memcpy(asmcode + 2, &func, 8);
	void* addr = GetProcAddress(GetModuleHandleA("NTDLL"), "DbgUiRemoteBreakin");
	WriteProcessMemory((HANDLE)-1, addr, asmcode, sizeof(asmcode), NULL);
#endif
}

#endif