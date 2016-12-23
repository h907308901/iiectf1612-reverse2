#define DEFINE_MYAPI_LIBS
#include "all.h"

TlsPtr* GetApiAddresses() {
	TlsPtr* tp = new TlsPtr;
	const MYAPI_LIB_INFO* libinfo;
	HMODULE hModule;
	char ApiName[64];
	int i, j;
	if (!tp)
		fatal("tp initialization failed");
	for (libinfo = MyApiLibs; libinfo < MyApiLibs + sizeof(MyApiLibs); libinfo++) {
		if ((hModule = GetModuleHandleA(libinfo->LibName)) == NULL &&
			(hModule = LoadLibraryA(libinfo->LibName)) == NULL)
			break;
		for (i = 0; strlen(libinfo->ApiInfo[i].EncryptedName) > 0; i++) {
			for (j = 0; libinfo->ApiInfo[i].EncryptedName[j] != '\0' && i < 63; j++) {
				ApiName[j] = (libinfo->ApiInfo[i].EncryptedName[j] ^ 'K') - 1;
			}
			ApiName[j] = '\0';
			tp->Store(libinfo->ApiInfo[i].Index, GetProcAddress(hModule, ApiName));
		}
	}
	return tp;
}