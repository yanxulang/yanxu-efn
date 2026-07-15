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


//type.h

#ifndef __REFLECT_TypeInfo_H__
#define __REFLECT_TypeInfo_H__

#include "reflect.h"
#include "customtypeloader.h"

#define CLS_reflect_TypeInfo  LIB_REFLECT myT(".") myT("类型信息")
#define IDX_reflect_TypeInfo  (IDX_sys_Object + 1)

///////////////////////////////////////////////////////////////////////////

inline void reflect_TypeInfo_ConstructFromObject(EFObject* pThis, EFObject* pObject)
{
	EF_CM_1(IDX_reflect_TypeInfo, 0, void, pThis, EFObject*, pObject);
}

inline void reflect_TypeInfo_ConstructFromTypeName(EFObject* pThis, EFText TypeName)
{
	EF_CM_1(IDX_reflect_TypeInfo, 1, void, pThis, EFText, TypeName);
}

inline void reflect_TypeInfo_ConstructFromTypeInfo(EFObject* pThis, EFObject* pOtherTypeInfo)
{
	EF_CM_1(IDX_reflect_TypeInfo, 2, void, pThis, EFObject*, pOtherTypeInfo);
}

inline void reflect_TypeInfo_ConstructFromTypeNameC(EFObject* pThis, EFChar* pTypeName)
{
	EF_CM_1(IDX_reflect_TypeInfo, 3, void, pThis, EFChar*, pTypeName);
}

inline void reflect_TypeInfo_ConstructFromBaseTypeInfo(EFObject* pThis, EFTypeInfo* pBaseTypeInfo, CustomTypeLoader* pTypeLoader)
{
	EF_CM_2(IDX_reflect_TypeInfo, 4, void, pThis, EFTypeInfo*, pBaseTypeInfo, CustomTypeLoader*, pTypeLoader);
}

inline EFText reflect_TypeInfo_GetName(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 5, EFText, pThis);
}

inline EFInt reflect_TypeInfo_GetNameHash(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 6, EFInt, pThis);
}

inline EFText reflect_TypeInfo_GetDescription(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 7, EFText, pThis);
}

inline EFText reflect_TypeInfo_GetExtendProperties(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 8, EFText, pThis);
}

inline EFBool reflect_TypeInfo_IsArray(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 9, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsBaseType(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 10, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsClass(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 11, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsInterface(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 12, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsEnum(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 13, EFBool, pThis);
}

inline EFObject* reflect_TypeInfo_GetParentType(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 14, EFObject*, pThis);
}

inline EFText reflect_TypeInfo_GetParentTypeName(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 15, EFText, pThis);
}

inline EFArray reflect_TypeInfo_GetImplementedInterfaces(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 16, EFArray, pThis);
}

inline EFArray reflect_TypeInfo_GetAllMethodInfos(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 17, EFArray, pThis);
}

inline EFArray reflect_TypeInfo_GetMethodInfos(EFObject* pThis, EFBool bWithPropertyMethods, EFBool bWithInterfaceMethods)
{
	return EF_CM_2(IDX_reflect_TypeInfo, 18, EFArray, pThis, EFBool, bWithPropertyMethods, EFBool, bWithInterfaceMethods);
}

inline EFArray reflect_TypeInfo_GetPropertyInfos(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 19, EFArray, pThis);
}

inline EFArray reflect_TypeInfo_GetExportEvents(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 20, EFArray, pThis);
}

inline EFArray reflect_TypeInfo_GetElementInfos(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 21, EFArray, pThis);
}

inline EFArray reflect_TypeInfo_GetEnumItemInfos(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 22, EFArray, pThis);
}

inline EFInt reflect_TypeInfo_GetState(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 23, EFInt, pThis);
}

inline EFBool reflect_TypeInfo_IsPublic(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 24, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsProtected(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 25, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsPrivate(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 26, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsHidden(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 27, EFBool, pThis);
}

inline EFBool reflect_TypeInfo_IsFinal(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 28, EFBool, pThis);
}

inline EFDouble reflect_TypeInfo_GetVersion(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 29, EFDouble, pThis);
}

inline EFInt reflect_TypeInfo_GetCategoryIndex(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 30, EFInt, pThis);
}

inline EFText reflect_TypeInfo_GetEnumItemName(EFObject* pThis, EFInt nEnumItemValue)
{
	return EF_CM_1(IDX_reflect_TypeInfo, 31, EFText, pThis, EFInt, nEnumItemValue);
}

inline EFText reflect_TypeInfo_ToString(EFObject* pThis)
{
	return EF_CM_0(IDX_reflect_TypeInfo, 32, EFText, pThis);
}

#endif //__REFLECT_TypeInfo_H__
