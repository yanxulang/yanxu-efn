/*
    版权所有 (C) 2007 大连大有吴涛易语言软件开发有限公司

    在满足下列所有许可条件的前提下，允许以源代码形式或编译后形式使用或重新发布本源文件的未经修改版本或修改后版本：
    1、如果以源代码形式重新发布，必须完整保留作者版权声明。
    2、如果以编译后形式重新发布，必须在发布版本的文档中重申作者版权声明。
    3、如果重新发布修改后版本，必须注明修改内容，必须完整保留或重申原作者版权声明。
    4、对本源文件的任何使用，必须满足以下要求之一：
       A、开发“易语言.飞扬”类库；
       B、开发用于“易语言.飞扬”的工具软件；
       C、开发用作辅助已有“易语言.飞扬”程序的软件。
    5：本源文件的作者不作任何承诺。任何情况下因任何原因而导致的任何损害，均不承担任何责任。
    6：必须保证本协议的完整性。
*/


#ifndef __EF_SYS_RUNTIME_H__
#define __EF_SYS_RUNTIME_H__

#include "runtime_err.h"

//////////////////////////////////////////////////////////////////////////

#define CLS_sys_Runtime  myT ("系统.运行环境")
const int IDX_sys_Runtime = 1;

#define CLS_sys_GC       myT ("系统.垃圾收集器")
const int IDX_sys_GC = 1;

//////////////////////////////////////////////////////////////////////////

#define sys_Runtime_Malloc  EF_malloc
#define sys_Runtime_Realloc EF_realloc
#define sys_Runtime_Free    EF_free

#ifndef _EF_SYS_LIB_

#define _EF_RUNTIME_SM_0(method_index, ret_datatype)    \
	(*(ret_datatype (*) ()) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) ();
#define _EF_RUNTIME_SM_1(method_index, ret_datatype, arg1_datatype, arg1_value)    \
	(*(ret_datatype (*) (arg1_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value);
#define _EF_RUNTIME_SM_2(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value);
#define _EF_RUNTIME_SM_3(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value);
#define _EF_RUNTIME_SM_4(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value);
#define _EF_RUNTIME_SM_5(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value);
#define _EF_RUNTIME_SM_6(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype)) ef_pRuntimeClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value);

inline void sys_Runtime_LockConstDataInfTab ()
{
	_EF_RUNTIME_SM_0 (0, void);
}

inline void sys_Runtime_UnlockConstDataInfTab ()
{
	_EF_RUNTIME_SM_0 (1, void);
}

inline void sys_Runtime_AddConstDataInf (EFConstDataHeader* pConstDataHeader)
{
	_EF_RUNTIME_SM_1 (2, void, EFConstDataHeader*, pConstDataHeader);
}

inline void sys_Runtime_RemoveConstDataInf (void* pLibInfo)
{
	_EF_RUNTIME_SM_1 (3, void, void*, pLibInfo);
}

inline EFClassInfo* sys_Runtime_GetGCClassInfo ()
{
	return _EF_RUNTIME_SM_0 (4, EFClassInfo*);
}

inline EFDWord sys_Runtime_CallExtendedMethod (EFInt nIndex, EFDWord dwParam1 = 0, EFDWord dwParam2 = 0, EFDWord dwParam3 = 0)
{
	return _EF_RUNTIME_SM_4 (5, EFDWord, EFInt, nIndex, EFDWord, dwParam1, EFDWord, dwParam2, EFDWord, dwParam3);
}

inline EFTypeInfo* sys_Runtime_LoadTypeInfo (EFChar* szTypeName)
{
	return _EF_RUNTIME_SM_1 (6, EFTypeInfo*, EFChar*, szTypeName);
}

inline EFClassInfo* sys_Runtime_LoadClassInfo (EFChar* szClassName)
{
	return _EF_RUNTIME_SM_1 (7, EFClassInfo*, EFChar*, szClassName);
}

inline EFObject* sys_Runtime_NewObject (EFText szClassName, EFBool blCallDefaultConstructor)
{
	return _EF_RUNTIME_SM_2 (11, EFObject*, EFText, szClassName, EFBool, blCallDefaultConstructor);
}

inline void sys_Runtime_ReportError (EFText ClsLibName, EFInt nErrorCode, EFText szErrorText)
{
	_EF_RUNTIME_SM_3 (12, void, EFText, ClsLibName, EFInt, nErrorCode, EFText, szErrorText);
}

inline void sys_Runtime_GetLastError (void* pLastErrorInf)
{
	_EF_RUNTIME_SM_1 (13, void, void*, pLastErrorInf);
}

inline void sys_Runtime_ClearError ()
{
	_EF_RUNTIME_SM_0 (14, void);
}

inline EFBool sys_Runtime_IsValidOject (EFObject* pObject)
{
	return _EF_RUNTIME_SM_1 (15, EFBool, EFObject*, pObject);
}

inline EFObject* sys_Runtime_NewObject2 (EFChar* szClassName, EFBool blCallDefaultConstructor)
{
	return _EF_RUNTIME_SM_2 (16, EFObject*, EFChar*, szClassName, EFBool, blCallDefaultConstructor);
}

inline void sys_Runtime_Exit (EFInt nExitCode)
{
	_EF_RUNTIME_SM_1 (17, void, EFInt, nExitCode);
}

inline EFInterface* sys_Runtime_SetErrorHandler (EFInterface* ErrorHandler)
{
	return _EF_RUNTIME_SM_1 (18, EFInterface*, EFInterface*, ErrorHandler);
}

inline EFBool sys_Runtime_Execute(EFText CommandLineText, EFText CurrentDirectory, EFBool bWait)
{
	return _EF_RUNTIME_SM_3 (19, EFBool, EFText, CommandLineText, EFText, CurrentDirectory, EFBool, bWait);
}


//////////////////////////////////////////////////////////////////////////
// 以下是隐藏在Runtime中的仅对C++用户公开的扩展功能（通过ExtendedMethod提供）

inline void sys_Runtime_ReportError2 (EFChar* szClsLibName, EFInt nErrorCode, EFChar* szErrorText)
{
	sys_Runtime_CallExtendedMethod (1, (EFDWord)szClsLibName, (EFDWord)nErrorCode, (EFDWord)szErrorText);
}

inline EFInterface* sys_Runtime_CreateInterface (EFObject* pObject, EFChar* szCreateInterfaceName)
{
	return (EFInterface*)sys_Runtime_CallExtendedMethod (2, (EFDWord)pObject, (EFDWord)szCreateInterfaceName, 0);
}

inline EFLibInfo* sys_Runtime_LoadClassLib (EFChar* szClassLibName)
{
	return (EFLibInfo*)sys_Runtime_CallExtendedMethod (3, (EFDWord)szClassLibName, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// GC

#define GCSM_0(method_index, ret_datatype)    \
	(*(ret_datatype (*) ()) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) ();
#define GCSM_1(method_index, ret_datatype, arg1_datatype, arg1_value)    \
	(*(ret_datatype (*) (arg1_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value);
#define GCSM_2(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value);
#define GCSM_3(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value);
#define GCSM_4(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value);
#define GCSM_5(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value);
#define GCSM_6(method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype)) ef_pGCClassInfo->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value);

inline void sys_GC_GC (EFInt nInterval)
{
	GCSM_1 (0, void, EFInt, nInterval);
}

//gcdata_type: DBT_*
inline void* sys_GC_RegisterData (void* pData, EFInt gcdata_type)
{
	return GCSM_2 (1, void*, void*, pData, EFInt, gcdata_type);
}

inline EFText sys_GC_CloneAndRegTextData (EFChar* szText)
{
	return GCSM_1 (2, EFText, EFChar*, szText);
}

inline EFBinary sys_GC_CloneAndRegBinData (void* pBinData, EFInt nDataLen)
{
	return GCSM_2 (3, EFBinary, void*, pBinData, EFInt, nDataLen);
}

inline void* sys_GC_IncRefCount (void* pData)
{
	return GCSM_1(4, void*, void*, pData);
}

inline void sys_GC_DecRefCount (void* pData)
{
	GCSM_1 (5, void, void*, pData);
}

inline void sys_GC_DumpObjects ()
{
	GCSM_0 (6, void);
}

#else

void Runtime_LockConstDataInfTab ();
void Runtime_UnlockConstDataInfTab ();
void Runtime_AddConstDataInf (EFConstDataHeader* pConstDataHeader);
void Runtime_RemoveConstDataInf (void* pLibInfo);
EFClassInfo* Runtime_GetGCClassInfo ();
EFDWord Runtime_ExtendedMethed (EFInt nIndex, EFDWord dwParam1 = 0, EFDWord dwParam2 = 0, EFDWord dwParam3 = 0);
EFTypeInfo* Runtime_LoadTypeInfo (EFChar* szTypeName);
EFClassInfo* Runtime_LoadClassInfo (EFChar* szClassName);
EFInt Runtime_Malloc (EFInt nReqMemSize);
EFInt Runtime_Realloc (void* pMem, EFInt nReallocMemSize);
void Runtime_Free (void* pMem);
EFObject* Runtime_NewObject (EFText szClassName, EFBool blCallDefaultConstructor);
void Runtime_ReportError (EFText ClsLibName, EFInt nErrorCode, EFText errorText);
extern void Runtime_GetLastError (void* pLastErrorInf);
extern void Runtime_ClearError ();
extern void ReportRuntimeError (EFChar* szClsLibName, EFInt nErrorCode, EFChar* szErrorText);
EFBool Runtime_IsValidOject (EFObject* pObject);
EFObject* Runtime_NewObject2 (EFChar* szClassName, EFBool blCallDefaultConstructor);
void Runtime_Exit (EFInt nExitCode);
EFInterface* EF_CreateInterface (EFObject* pObject, EFChar* szCreateInterfaceName);

#define sys_Runtime_LockConstDataInfTab     Runtime_LockConstDataInfTab
#define sys_Runtime_UnlockConstDataInfTab   Runtime_UnlockConstDataInfTab
#define sys_Runtime_AddConstDataInf         Runtime_AddConstDataInf
#define sys_Runtime_RemoveConstDataInf      Runtime_RemoveConstDataInf
#define sys_Runtime_GetGCClassInfo          Runtime_GetGCClassInfo
#define sys_Runtime_CallExtendedMethod      Runtime_ExtendedMethed;
#define sys_Runtime_LoadTypeInfo            Runtime_LoadTypeInfo
#define sys_Runtime_LoadClassInfo           Runtime_LoadClassInfo
#define sys_Runtime_NewObject               Runtime_NewObject
#define sys_Runtime_ReportError             Runtime_ReportError
#define sys_Runtime_GetLastError            Runtime_GetLastError
#define sys_Runtime_ClearError              Runtime_ClearError
#define sys_Runtime_IsValidOject            Runtime_IsValidOject
#define sys_Runtime_NewObject2              Runtime_NewObject2
#define sys_Runtime_ReportError2            ReportRuntimeError
#define sys_Runtime_CreateInterface         EF_CreateInterface
#define sys_Runtime_Exit                    Runtime_Exit
#define sys_Runtime_SetErrorHandler         Runtime_SetErrorHandler
#define sys_Runtime_Execute                 Runtime_Execute

//////////////////////////////////////////////////////////////////////////
// GC

void GC_GC (EFInt nInterval);

void* MIncRefCount (void* pData);
void  MDecRefCount (void* pData, EFBool bIsCalledByGC);
void* MRegisterDataToGC (void* pData, EFInt gcdata_type);
EFText MCloneAndRegEText (EFChar* ps);
EFBinary MCloneAndRegEBin (void* pData, EFInt nDataLength);
EFBool MIsValidManagedData (void* pData);
void MDumpHandleTable (EFBool blDumpLeakMode);

#define sys_GC_GC                   GC_GC
#define sys_GC_RegisterData         MRegisterDataToGC
#define sys_GC_CloneAndRegTextData  MCloneAndRegEText
#define sys_GC_CloneAndRegBinData   MCloneAndRegEBin
#define sys_GC_IncRefCount          MIncRefCount
#define sys_GC_DecRefCount(pData)   MDecRefCount (pData, EF_FALSE)
#define sys_GC_DumpObjects          MDumpHandleTable (EF_FALSE)

#endif

//////////////////////////////////////////////////////////////////////////

#define I_sys_ErrorHandler myT("系统.错误处理")

inline EFBool sys_ErrorHandler_ProcessRuntimeError (EFInterface* pThis, EFInt nErrorCode, EFText ErrorText)
{
	return EF_IM_2 (pThis, 0, EFBool, EFInt, nErrorCode, EFText, ErrorText);
}

#endif //__EF_SYS_RUNTIME_H__
