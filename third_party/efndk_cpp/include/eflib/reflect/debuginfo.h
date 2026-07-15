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


#ifndef __Reflect_DebugInfo_H__
#define __Reflect_DebugInfo_H__

#include "reflect.h"
#include "customtypeloader.h"


#define CLS_reflect_MethodDebugInfo  LIB_REFLECT myT(".") myT("方法调试信息")
#define IDX_reflect_MethodDebugInfo  (IDX_sys_Object + 1)

#define CLS_reflect_StatementLineDebugInfo  LIB_REFLECT myT(".") myT("代码行调试信息")
#define IDX_reflect_StatementLineDebugInfo  (IDX_sys_Object + 1)

#define CLS_reflect_CodeBlockDebugInfo  LIB_REFLECT myT(".") myT("代码块调试信息")
#define IDX_reflect_CodeBlockDebugInfo  (IDX_sys_Object + 1)

#define CLS_reflect_LocalVarDebugInfo  LIB_REFLECT myT(".") myT("变量调试信息")
#define IDX_reflect_LocalVarDebugInfo  (IDX_sys_Object + 1)


//////////////////////////////////////////////////////////////////////////

inline void reflect_MethodDebugInfo_Construct(EFObject* pThis, EFMethodDebugInfo* pDebugInfo)
{
	EF_CM_1(IDX_reflect_MethodDebugInfo, 0, void, pThis, EFMethodDebugInfo*, pDebugInfo);
}

inline void reflect_StatementLineDebugInfo_Construct(EFObject* pThis, EFStatementLineDebugInfo* pDebugInfo)
{
	EF_CM_1(IDX_reflect_StatementLineDebugInfo, 0, void, pThis, EFStatementLineDebugInfo*, pDebugInfo);
}

inline void reflect_CodeBlockDebugInfo_Construct(EFObject* pThis, EFCodeBlockDebugInfo* pDebugInfo)
{
	EF_CM_1(IDX_reflect_CodeBlockDebugInfo, 0, void, pThis, EFCodeBlockDebugInfo*, pDebugInfo);
}

inline void reflect_LocalVarDebugInfo_Construct(EFObject* pThis, EFLocalVarDebugInfo* pDebugInfo)
{
	EF_CM_1(IDX_reflect_LocalVarDebugInfo, 0, void, pThis, EFLocalVarDebugInfo*, pDebugInfo);
}


#endif //__Reflect_DebugInfo_H__
