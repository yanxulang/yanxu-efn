#include "lib_yanxu.h"
#include "yanxu_loader.h"

static void Yanxu_OnUnload(EFLibInfo*)
{
    Yanxu_FreeAllEngines();
}

EF_DEFINE_LIB_INFO
(
    1.0, 1,
    yanxu, LIB_YANXU,
    myT("言序 EF 类库：通过 yanxu C ABI 动态执行言序源码并返回 JSON 结果。"),
    0,
    myT(""),
    EF_LANGUAGE_CHINESE, EF_OS_ALL,
    myT(""),
    myT("libyanxu.so;yanxu.dll;libyanxu.dylib"),
    myT("YanXuLang"),
    myT("https://yanxu.dev"),
    NULL
)

EF_DEFINE_DEFAULT_NOTIFY_LIB_FUNC(NULL, Yanxu_OnUnload)
