#ifndef __YANXU_EFN_LOADER__
#define __YANXU_EFN_LOADER__

#include "lib_yanxu.h"

typedef void YanxuEngine;

EFBool Yanxu_LoadRuntime(EFText path);
EFBool Yanxu_IsRuntimeLoaded();
EFText Yanxu_LastError();
EFText Yanxu_RuntimePath();

EFInt Yanxu_CreateEngine(EFBool unrestricted);
EFText Yanxu_RunEngine(EFInt handle, EFText source);
EFBool Yanxu_FreeEngine(EFInt handle);
void Yanxu_FreeAllEngines();

#endif
