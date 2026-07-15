#include "yanxu_loader.h"

#include <map>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

typedef YanxuEngine* (*PFN_yanxu_engine_new)();
typedef YanxuEngine* (*PFN_yanxu_engine_new_unrestricted)();
typedef char* (*PFN_yanxu_engine_run)(YanxuEngine*, const char*);
typedef void (*PFN_yanxu_engine_free)(YanxuEngine*);
typedef void (*PFN_yanxu_string_free)(char*);

static void* g_runtime = 0;
static std::string g_runtime_path;
static std::string g_last_error;
static PFN_yanxu_engine_new g_engine_new = 0;
static PFN_yanxu_engine_new_unrestricted g_engine_new_unrestricted = 0;
static PFN_yanxu_engine_run g_engine_run = 0;
static PFN_yanxu_engine_free g_engine_free = 0;
static PFN_yanxu_string_free g_string_free = 0;
static EFInt g_next_handle = 1;
static std::map<EFInt, YanxuEngine*> g_engines;

static void set_error(const char* message)
{
    g_last_error = message ? message : "";
}

static std::string eftext_to_utf8(EFText text)
{
    if (text == NULL)
        return std::string();
#ifdef _UNICODE
    EF_MiniMem mem;
    char* utf8 = EF_UCS_to_UTF8((wchar_t*)EF_GET_TEXT(text), EF_GET_TEXT_LEN(text), mem);
    return utf8 ? std::string(utf8) : std::string();
#else
    return std::string((char*)EF_GET_TEXT(text), EF_GET_TEXT_LEN(text));
#endif
}

static EFText utf8_to_eftext(const char* text)
{
    if (text == NULL)
        return EF_CloneEFText(myT(""));
#ifdef _UNICODE
    EF_MiniMem mem;
    wchar_t* wide = EF_UTF8_to_UCS((char*)text, -1, mem);
    return EF_CloneEFText((EFChar*)wide);
#else
    return EF_CloneEFText((char*)text);
#endif
}

static void* load_symbol(const char* name)
{
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)g_runtime, name);
#else
    return dlsym(g_runtime, name);
#endif
}

static EFBool bind_symbols()
{
    g_engine_new = (PFN_yanxu_engine_new)load_symbol("yanxu_engine_new");
    g_engine_new_unrestricted =
        (PFN_yanxu_engine_new_unrestricted)load_symbol("yanxu_engine_new_unrestricted");
    g_engine_run = (PFN_yanxu_engine_run)load_symbol("yanxu_engine_run");
    g_engine_free = (PFN_yanxu_engine_free)load_symbol("yanxu_engine_free");
    g_string_free = (PFN_yanxu_string_free)load_symbol("yanxu_string_free");
    if (!g_engine_new || !g_engine_new_unrestricted || !g_engine_run || !g_engine_free ||
        !g_string_free)
    {
        set_error("yanxu runtime ABI is incomplete");
        return EF_FALSE;
    }
    return EF_TRUE;
}

EFBool Yanxu_LoadRuntime(EFText path)
{
    std::string native_path = eftext_to_utf8(path);
    if (native_path.empty())
    {
        set_error("runtime path is empty");
        return EF_FALSE;
    }
    if (g_runtime)
        return EF_TRUE;

#ifdef _WIN32
    g_runtime = (void*)LoadLibraryA(native_path.c_str());
    if (!g_runtime)
    {
        set_error("LoadLibraryA failed");
        return EF_FALSE;
    }
#else
    g_runtime = dlopen(native_path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (!g_runtime)
    {
        set_error(dlerror());
        return EF_FALSE;
    }
#endif
    g_runtime_path = native_path;
    return bind_symbols();
}

EFBool Yanxu_IsRuntimeLoaded()
{
    return g_runtime ? EF_TRUE : EF_FALSE;
}

EFText Yanxu_LastError()
{
    return utf8_to_eftext(g_last_error.c_str());
}

EFText Yanxu_RuntimePath()
{
    return utf8_to_eftext(g_runtime_path.c_str());
}

EFInt Yanxu_CreateEngine(EFBool unrestricted)
{
    if (!g_runtime)
    {
        set_error("yanxu runtime is not loaded");
        return 0;
    }
    YanxuEngine* engine =
        unrestricted ? g_engine_new_unrestricted() : g_engine_new();
    if (!engine)
    {
        set_error("yanxu_engine_new returned null");
        return 0;
    }
    EFInt handle = g_next_handle++;
    if (handle == 0)
        handle = g_next_handle++;
    g_engines[handle] = engine;
    return handle;
}

EFText Yanxu_RunEngine(EFInt handle, EFText source)
{
    std::map<EFInt, YanxuEngine*>::iterator it = g_engines.find(handle);
    if (it == g_engines.end())
        return utf8_to_eftext("{\"schema\":1,\"ok\":false,\"kind\":\"host\",\"message\":\"invalid engine handle\"}");

    std::string source_utf8 = eftext_to_utf8(source);
    char* result = g_engine_run(it->second, source_utf8.c_str());
    if (!result)
        return utf8_to_eftext("{\"schema\":1,\"ok\":false,\"kind\":\"host\",\"message\":\"yanxu_engine_run returned null\"}");
    EFText ef_result = utf8_to_eftext(result);
    g_string_free(result);
    return ef_result;
}

EFBool Yanxu_FreeEngine(EFInt handle)
{
    std::map<EFInt, YanxuEngine*>::iterator it = g_engines.find(handle);
    if (it == g_engines.end())
        return EF_FALSE;
    g_engine_free(it->second);
    g_engines.erase(it);
    return EF_TRUE;
}

void Yanxu_FreeAllEngines()
{
    for (std::map<EFInt, YanxuEngine*>::iterator it = g_engines.begin(); it != g_engines.end();
         ++it)
    {
        if (it->second)
            g_engine_free(it->second);
    }
    g_engines.clear();
}
