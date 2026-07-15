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


//methodinfo.h

#ifndef __METHODINFO_H__
#define __METHODINFO_H__

#include "reflect.h"
#include "customtypeloader.h"

#define CLS_reflect_MethodInfo  LIB_REFLECT myT(".") myT("方法信息")
#define IDX_reflect_MethodInfo  (IDX_sys_Object + 1)

#define CLS_reflect_PropertyInfo  LIB_REFLECT myT(".") myT("属性信息")
#define IDX_reflect_PropertyInfo  (IDX_sys_Object + 1)

//////////////////////////////////////////////////////////////////////////

inline void reflect_MethodInfo_ConstructFromMethodItem(EFObject* pThis, EFMethodInfo* pMethodInfo, CustomTypeLoader* pTypeLoader)
{
	EF_CM_2(IDX_reflect_MethodInfo, 0, EFText, pThis, EFMethodInfo*, pMethodInfo, CustomTypeLoader*, pTypeLoader);
}


///////////////////////////////////////////////////////////////////////////////

inline void reflect_PropertyInfo_ConstructFromMethodItem(EFObject* pThis, EFMethodInfo* pPropertyGetMethodItem, EFMethodInfo* pPropertySetMethodItem, CustomTypeLoader* pTypeLoader)
{
	EF_CM_3(IDX_reflect_PropertyInfo, 0, EFText, pThis, EFMethodInfo*, pPropertyGetMethodItem, EFMethodInfo*, pPropertySetMethodItem, CustomTypeLoader*, pTypeLoader);
}

inline EFText reflect_PropertyInfo_GetName(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 1, EFText, pThis);
}

inline EFText reflect_PropertyInfo_GetTypeName(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 2, EFText, pThis);
}

inline EFObject* reflect_PropertyInfo_GetTypeInfo(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 3, EFObject*, pThis);
}

inline EFBool reflect_PropertyInfo_GetIsGet(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 4, EFBool, pThis);
}

inline EFBool reflect_PropertyInfo_GetIsSet(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 5, EFBool, pThis);
}

inline EFObject* reflect_PropertyInfo_GetGetter(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 6, EFObject*, pThis);
}

inline EFObject* reflect_PropertyInfo_GetSetter(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 7, EFObject*, pThis);
}

inline EFText reflect_PropertyInfo_ToString(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_PropertyInfo, 8, EFText, pThis);
}


#endif //__METHODINFO_H__
