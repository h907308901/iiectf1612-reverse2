#pragma once
#include "tlsptr.h"

#ifndef DEBUG_BUILD

void DebuggerCheck(TlsPtr* tp);
void FixImage();
void PatchBreakin();

#endif