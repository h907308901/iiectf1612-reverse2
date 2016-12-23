#pragma once

// This header file simply includes dependencies and other header files and some common definitions

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ImageHlp.h>

// Anti-debug is disabled if this macro is defined
//#define DEBUG_BUILD

#include "antidbg.h"
#include "bytecode.h"
#include "myapi.h"
#include "tlsptr.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "imagehlp.lib")

#define fatal(x) FatalAppExitA(0, x)

// These are native APIs
typedef DWORD(*NTQUERYINFORMATIONPROCESS)(HANDLE, ULONG, PVOID, ULONG, PULONG);
typedef DWORD(*NTREMOVEPROCESSDEBUG)(HANDLE, HANDLE);