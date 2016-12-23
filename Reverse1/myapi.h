#pragma once
#include "tlsptr.h"

// This module fetches some secret APIs' address and put then into TlsPtr
// I do all of these to make it difficult to find out what APIs I have used
// I do not want to explain much because I think everyone will understand them through the code

// These indexes are the identifier in TlsPtr
enum MYAPI_INDEX {
	API_QUERYINFORMATIONPROCESS,
	API_REMOVEPROCESSDEBUG,
	API_ENUM_END
};

struct MYAPI_API_INFO {
	MYAPI_INDEX Index;
	char EncryptedName[64];
};

const int MYAPI_MAX_API_COUNT = 32;

struct MYAPI_LIB_INFO {
	const char LibName[32];
	MYAPI_API_INFO ApiInfo[MYAPI_MAX_API_COUNT];
};

TlsPtr* GetApiAddresses();

#ifdef DEFINE_MYAPI_LIBS
static const MYAPI_LIB_INFO MyApiLibs[] = {
	// NTDLL
	{
		"NTDLL",
		{
			{ API_QUERYINFORMATIONPROCESS, "\x04\x3E\x19\x3D\x2D\x38\x31\x01\x24\x2C\x3B\x38\x25\x29\x3E\x21\x3B\x24\x1A\x38\x3B\x2F\x2D\x3F\x3F" },
			{ API_REMOVEPROCESSDEBUG, "\x04\x3E\x18\x2D\x25\x3B\x3C\x2D\x1A\x38\x3B\x2F\x2D\x3F\x3F\x0E\x2D\x28\x3D\x23" },
		}
	},
};
#endif