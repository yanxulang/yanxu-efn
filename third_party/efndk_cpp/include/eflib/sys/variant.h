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


#ifndef __EF_SYS_VARIANT_H__
#define __EF_SYS_VARIANT_H__

#include <ef/ef.h>
#include <ef/utils.h>

#define CLS_sys_Variant        myT("系统.通用型")
#define CLS_sys_NumberVariant  myT("系统.数值型")
#define CLS_sys_ArrayVariant   myT("系统.数组型")

#define IDX_sys_Variant  (IDX_sys_Object + 1)
#define IDX_sys_NumberVariant  (IDX_sys_Object + 1)
#define IDX_sys_ArrayVariant  (IDX_sys_Object + 1)

#define ENUM_sys_DataType  myT("系统.数据类型")


//////////////////////////////////////////////////////////////////////////

enum VariantType 
{
	vt_Int = 0, vt_Double, vt_Bool, vt_Text, vt_Binary, vt_Object, vt_Interface, vt_Enum,
	vt_IntArray, vt_DoubleArray, vt_BoolArray, vt_TextArray, vt_BinaryArray, vt_ObjectArray, vt_InterfaceArray, vt_EnumArray,
};

//////////////////////////////////////////////////////////////////////////

//---- Variant ----//

//Variant Constructors

inline void sys_Variant_ConstructFromInt(EFObject* pThis, EFInt value)
{
	EF_CM_1(IDX_sys_Variant, 0, void, pThis, EFInt, value);
}

inline void sys_Variant_ConstructFromDouble(EFObject* pThis, EFDouble value)
{
	EF_CM_1(IDX_sys_Variant, 1, void, pThis, EFDouble, value);
}

inline void sys_Variant_ConstructFromBool(EFObject* pThis, EFBool value)
{
	EF_CM_1(IDX_sys_Variant, 2, void, pThis, EFBool, value);
}

inline void sys_Variant_ConstructFromText(EFObject* pThis, EFText value)
{
	EF_CM_1(IDX_sys_Variant, 3, void, pThis, EFText, value);
}

inline void sys_Variant_ConstructFromBinary(EFObject* pThis, EFBinary value)
{
	EF_CM_1(IDX_sys_Variant, 4, void, pThis, EFBinary, value);
}

inline void sys_Variant_ConstructFromObject(EFObject* pThis, EFObject* value)
{
	EF_CM_1(IDX_sys_Variant, 5, void, pThis, EFObject*, value);
}

inline void sys_Variant_ConstructFromInterface(EFObject* pThis, EFInterface* value)
{
	EF_CM_1(IDX_sys_Variant, 6, void, pThis, EFInterface*, value);
}

//Variant Constructors for Array

inline void sys_Variant_ConstructFromIntArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 7, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromDoubleArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 8, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromBoolArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 9, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromTextArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 10, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromBinaryArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 11, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromObjectArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 12, void, pThis, EFArray, value);
}

inline void sys_Variant_ConstructFromInterfaceArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 13, void, pThis, EFArray, value);
}

//Variant GetType

inline VariantType sys_Variant_GetType(EFObject* pThis)
{
	return (VariantType) EF_CM_0(IDX_sys_Variant, 14, EFInt, pThis);
}

//Variant Getters

inline EFInt sys_Variant_GetInt(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 15, EFInt, pThis);
}

inline EFDouble sys_Variant_GetDouble(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 16, EFDouble, pThis);
}

inline EFBool sys_Variant_GetBool(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 17, EFBool, pThis);
}

inline EFText sys_Variant_GetText(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 18, EFText, pThis);
}

inline EFBinary sys_Variant_GetBinary(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 19, EFBinary, pThis);
}

inline EFObject* sys_Variant_GetObject(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 20, EFObject*, pThis);
}

inline EFInterface* sys_Variant_GetInterface(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 21, EFInterface*, pThis);
}

//Variant Getters for Array

inline EFArray sys_Variant_GetArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 22, EFArray, pThis);
}

inline EFArray sys_Variant_GetIntArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 23, EFArray, pThis);
}

inline EFArray sys_Variant_GetDoubleArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 24, EFArray, pThis);
}

inline EFArray sys_Variant_GetBoolArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 25, EFArray, pThis);
}

inline EFArray sys_Variant_GetTextArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 26, EFArray, pThis);
}

inline EFArray sys_Variant_GetBinaryArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 27, EFArray, pThis);
}

inline EFArray sys_Variant_GetObjectArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 28, EFArray, pThis);
}

inline EFArray sys_Variant_GetInterfaceArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 29, EFArray, pThis);
}

//Variant Setters

inline void sys_Variant_SetInt(EFObject* pThis, EFInt value)
{
	EF_CM_1(IDX_sys_Variant, 30, void, pThis, EFInt, value);
}

inline void sys_Variant_SetDouble(EFObject* pThis, EFDouble value)
{
	EF_CM_1(IDX_sys_Variant, 31, void, pThis, EFDouble, value);
}

inline void sys_Variant_SetBool(EFObject* pThis, EFBool value)
{
	EF_CM_1(IDX_sys_Variant, 32, void, pThis, EFBool, value);
}

inline void sys_Variant_SetText(EFObject* pThis, EFText value)
{
	EF_CM_1(IDX_sys_Variant, 33, void, pThis, EFText, value);
}

inline void sys_Variant_SetBinary(EFObject* pThis, EFBinary value)
{
	EF_CM_1(IDX_sys_Variant, 34, void, pThis, EFBinary, value);
}

inline void sys_Variant_SetObject(EFObject* pThis, EFObject* value)
{
	EF_CM_1(IDX_sys_Variant, 35, void, pThis, EFObject*, value);
}

inline void sys_Variant_SetInterface(EFObject* pThis, EFInterface* value)
{
	EF_CM_1(IDX_sys_Variant, 36, void, pThis, EFInterface*, value);
}

//Variant Setters for Array

inline void sys_Variant_SetIntArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 37, void, pThis, EFArray, value);
}

inline void sys_Variant_SetDoubleArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 38, void, pThis, EFArray, value);
}

inline void sys_Variant_SetBoolArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 39, void, pThis, EFArray, value);
}

inline void sys_Variant_SetTextArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 40, void, pThis, EFArray, value);
}

inline void sys_Variant_SetBinaryArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 41, void, pThis, EFArray, value);
}

inline void sys_Variant_SetObjectArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 42, void, pThis, EFArray, value);
}

inline void sys_Variant_SetInterfaceArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_Variant, 43, void, pThis, EFArray, value);
}

//Others

inline EFBool sys_Variant_Equals(EFObject* pThis, EFObject* pObject)
{
	return EF_CM_1(IDX_sys_Variant, 44, EFBool, pThis, EFObject*, pObject);
}

inline EFText sys_Variant_ToString(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_Variant, 45, EFText, pThis);
}


//////////////////////////////////////////////////////////////////////////

//---- NumberVariant ----//

inline void sys_NumberVariant_ConstructFromInt(EFObject* pThis, EFInt value)
{
	EF_CM_1(IDX_sys_NumberVariant, 0, void, pThis, EFInt, value);
}

inline void sys_NumberVariant_ConstructFromDouble(EFObject* pThis, EFDouble value)
{
	EF_CM_1(IDX_sys_NumberVariant, 1, void, pThis, EFDouble, value);
}

inline VariantType sys_NumberVariant_GetType(EFObject* pThis)
{
	return (VariantType) EF_CM_0(IDX_sys_NumberVariant, 2, VariantType, pThis);
}

inline EFInt sys_NumberVariant_GetInt(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_NumberVariant, 3, EFInt, pThis);
}

inline EFDouble sys_NumberVariant_GetDouble(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_NumberVariant, 4, EFDouble, pThis);
}

inline void sys_NumberVariant_SetInt(EFObject* pThis, EFInt value)
{
	EF_CM_1(IDX_sys_NumberVariant, 5, void, pThis, EFInt, value);
}

inline void sys_NumberVariant_SetDouble(EFObject* pThis, EFDouble value)
{
	EF_CM_1(IDX_sys_NumberVariant, 6, void, pThis, EFDouble, value);
}

inline EFBool sys_NumberVariant_Equals(EFObject* pThis, EFObject* pObject)
{
	return EF_CM_1(IDX_sys_NumberVariant, 7, EFBool, pThis, EFObject*, pObject);
}

inline EFText sys_NumberVariant_ToString(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_NumberVariant, 8, EFText, pThis);
}



//////////////////////////////////////////////////////////////////////////

//---- ArrayVariant ----//

//ArrayVariant Constructors

inline void sys_ArrayVariant_ConstructFromIntArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 0, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromDoubleArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 1, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromBoolArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 2, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromTextArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 3, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromBinaryArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 4, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromObjectArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 5, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_ConstructFromInterfaceArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 6, void, pThis, EFArray, value);
}

//ArrayVariant GetType

inline VariantType sys_ArrayVariant_GetType(EFObject* pThis)
{
	return (VariantType) EF_CM_0(IDX_sys_ArrayVariant, 7, EFInt, pThis);
}

//ArrayVariant Getters

inline EFArray sys_ArrayVariant_GetArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 8, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetIntArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 9, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetDoubleArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 10, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetBoolArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 11, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetTextArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 12, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetBinaryArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 13, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetObjectArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 14, EFArray, pThis);
}

inline EFArray sys_ArrayVariant_GetInterfaceArray(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 15, EFArray, pThis);
}

//ArrayVariant Setters

inline void sys_ArrayVariant_SetIntArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 16, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetDoubleArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 17, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetBoolArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 18, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetTextArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 19, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetBinaryArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 20, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetObjectArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 21, void, pThis, EFArray, value);
}

inline void sys_ArrayVariant_SetInterfaceArray(EFObject* pThis, EFArray value)
{
	EF_CM_1(IDX_sys_ArrayVariant, 22, void, pThis, EFArray, value);
}

//ArrayVariant Others

inline EFBool sys_ArrayVariant_Equals(EFObject* pThis, EFObject* pObject)
{
	return EF_CM_1(IDX_sys_ArrayVariant, 23, EFBool, pThis, EFObject*, pObject);
}

inline EFText sys_ArrayVariant_ToString(EFObject* pThis)
{
	return EF_CM_0(IDX_sys_ArrayVariant, 24, EFText, pThis);
}


#endif //__EF_SYS_VARIANT_H__
