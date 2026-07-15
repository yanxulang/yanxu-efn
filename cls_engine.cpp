#include "cls_engine.h"
#include "yanxu_loader.h"

static YanxuEngineFields* Engine_fields(EFObject* pThis)
{
    return (YanxuEngineFields*)EF_GET_OBJECT_FIELDS_DATA_ADDRESS(pThis, IDX_yanxu_Engine);
}

static void Engine_init(EFObject* pThis)
{
    Engine_fields(pThis)->handle = 0;
}

static void Engine_clean(EFObject* pThis)
{
    YanxuEngineFields* fields = Engine_fields(pThis);
    if (fields->handle != 0)
    {
        Yanxu_FreeEngine(fields->handle);
        fields->handle = 0;
    }
}

EF_BEGIN_DEFINE_FIELD_TABLE(Engine)
    EF_FIELD_OF_C_POINTER,
EF_END_DEFINE_FIELD_TABLE

EF_BEGIN_DEFINE_ARG_TABLE(Engine, LoadRuntime)
    EF_ARG(myT("运行时路径"), myT("libyanxu.so、yanxu.dll 或 libyanxu.dylib 的路径"), 0, EF_TEXT),
EF_END_DEFINE_ARG_TABLE
EFBool Engine_LoadRuntime(EFText path)
{
    return Yanxu_LoadRuntime(path);
}

EFBool Engine_IsRuntimeLoaded()
{
    return Yanxu_IsRuntimeLoaded();
}

EFText Engine_LastError()
{
    return Yanxu_LastError();
}

EFText Engine_RuntimePath()
{
    return Yanxu_RuntimePath();
}

EF_BEGIN_DEFINE_ARG_TABLE(Engine, NewSandboxed)
    EF_ARG(myT("运行时路径"), myT("首次调用时载入；已载入时可传空文本"), EF_S_ARG_NULLABLE, EF_TEXT),
EF_END_DEFINE_ARG_TABLE
EFBool Engine_NewSandboxed(EFObject* pThis, EFText runtime_path)
{
    if (!Yanxu_IsRuntimeLoaded() && !Yanxu_LoadRuntime(runtime_path))
        return EF_FALSE;
    YanxuEngineFields* fields = Engine_fields(pThis);
    if (fields->handle != 0)
        Yanxu_FreeEngine(fields->handle);
    fields->handle = Yanxu_CreateEngine(EF_FALSE);
    return fields->handle != 0 ? EF_TRUE : EF_FALSE;
}

EF_BEGIN_DEFINE_ARG_TABLE(Engine, NewUnrestricted)
    EF_ARG(myT("运行时路径"), myT("首次调用时载入；已载入时可传空文本"), EF_S_ARG_NULLABLE, EF_TEXT),
EF_END_DEFINE_ARG_TABLE
EFBool Engine_NewUnrestricted(EFObject* pThis, EFText runtime_path)
{
    if (!Yanxu_IsRuntimeLoaded() && !Yanxu_LoadRuntime(runtime_path))
        return EF_FALSE;
    YanxuEngineFields* fields = Engine_fields(pThis);
    if (fields->handle != 0)
        Yanxu_FreeEngine(fields->handle);
    fields->handle = Yanxu_CreateEngine(EF_TRUE);
    return fields->handle != 0 ? EF_TRUE : EF_FALSE;
}

EF_BEGIN_DEFINE_ARG_TABLE(Engine, Run)
    EF_ARG(myT("源码"), myT("言序源码，UTF-8 JSON 结果会转回 EF 文本"), 0, EF_TEXT),
EF_END_DEFINE_ARG_TABLE
EFText Engine_Run(EFObject* pThis, EFText source)
{
    return Yanxu_RunEngine(Engine_fields(pThis)->handle, source);
}

EFBool Engine_Free(EFObject* pThis)
{
    YanxuEngineFields* fields = Engine_fields(pThis);
    EFBool ok = Yanxu_FreeEngine(fields->handle);
    fields->handle = 0;
    return ok;
}

EF_BEGIN_DEFINE_ARG_TABLE(Engine, RunSandboxed)
    EF_ARG(myT("运行时路径"), myT("libyanxu.so、yanxu.dll 或 libyanxu.dylib 的路径"), 0, EF_TEXT),
    EF_ARG(myT("源码"), myT("言序源码"), 0, EF_TEXT),
EF_END_DEFINE_ARG_TABLE
EFText Engine_RunSandboxed(EFText runtime_path, EFText source)
{
    if (!Yanxu_IsRuntimeLoaded() && !Yanxu_LoadRuntime(runtime_path))
        return Yanxu_LastError();
    EFInt handle = Yanxu_CreateEngine(EF_FALSE);
    EFText result = Yanxu_RunEngine(handle, source);
    Yanxu_FreeEngine(handle);
    return result;
}

EF_BEGIN_DEFINE_METHOD_TABLE(Engine)
    EF_METHOD(Engine, LoadRuntime, myT("载入运行时"), myT("动态载入 yanxu C ABI 动态库"), myT(""), EF_S_PUBLIC|EF_S_M_STATIC, EF_BOOL),
    EF_METHOD_NOARG(Engine, IsRuntimeLoaded, myT("运行时已载入"), myT(""), myT(""), EF_S_PUBLIC|EF_S_M_STATIC, EF_BOOL),
    EF_METHOD_NOARG(Engine, LastError, myT("取最后错误"), myT(""), myT(""), EF_S_PUBLIC|EF_S_M_STATIC, EF_TEXT),
    EF_METHOD_NOARG(Engine, RuntimePath, myT("取运行时路径"), myT(""), myT(""), EF_S_PUBLIC|EF_S_M_STATIC, EF_TEXT),
    EF_METHOD(Engine, NewSandboxed, myT("新建沙箱引擎"), myT("默认无宿主权限，适合不可信源码"), myT(""), EF_S_PUBLIC, EF_BOOL),
    EF_METHOD(Engine, NewUnrestricted, myT("新建不受限引擎"), myT("仅可信源码使用"), myT(""), EF_S_PUBLIC, EF_BOOL),
    EF_METHOD(Engine, Run, myT("运行"), myT("在持久引擎中执行言序源码，返回 schema 1 JSON"), myT(""), EF_S_PUBLIC, EF_TEXT),
    EF_METHOD_NOARG(Engine, Free, myT("释放"), myT("释放当前对象持有的言序引擎"), myT(""), EF_S_PUBLIC, EF_BOOL),
    EF_METHOD(Engine, RunSandboxed, myT("沙箱运行"), myT("一次性创建沙箱引擎并运行源码"), myT(""), EF_S_PUBLIC|EF_S_M_STATIC, EF_TEXT),
EF_END_DEFINE_METHOD_TABLE

EF_DEFINE_CLASS_INFO
(
    1.0,
    Engine, CLS_yanxu_Engine,
    myT("动态调用 yanxu 程序和源码的 EF 类库"),
    myT("系统.对象"),
    0, NULL,
    EF_S_PUBLIC, -1,
    myT(""),
    NULL,
    Engine_init,
    Engine_clean,
    EF_METHOD_COUNT(Engine), EF_METHOD_TABLE(Engine),
    EF_FIELD_COUNT(Engine), EF_FIELD_TABLE(Engine),
    0, NULL
)
