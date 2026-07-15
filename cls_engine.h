#ifndef __YANXU_EFN_ENGINE__
#define __YANXU_EFN_ENGINE__

#include "lib_yanxu.h"

#define CLS_yanxu_Engine LIB_YANXU myT(".") myT("引擎")
#define IDX_yanxu_Engine (IDX_sys_Object + 1)

struct YanxuEngineFields
{
    EFInt handle;
};

#endif
