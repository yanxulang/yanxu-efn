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


#ifndef __EF_SYS_BASE_TYPES__
#define __EF_SYS_BASE_TYPES__

#include <eflib/sys/object.h>

//定义几个特殊的类, 用于实现类似如下用法: "abc".length()

#define CLS_sys_Integer    myT ("系统.整数类")
#define CLS_sys_Double     myT ("系统.小数类")
#define CLS_sys_Boolean    myT ("系统.逻辑类")
#define CLS_sys_Text       myT ("系统.文本类")
#define CLS_sys_Binary     myT ("系统.字节集类")

#define CLS_sys_Array      myT ("系统.数组类")


#define IDX_sys_Integer    (IDX_sys_Object + 1)
#define IDX_sys_Double     (IDX_sys_Object + 1)
#define IDX_sys_Boolean    (IDX_sys_Object + 1)
#define IDX_sys_Text       (IDX_sys_Object + 1)
#define IDX_sys_Binary     (IDX_sys_Object + 1)

#define IDX_sys_Array      (IDX_sys_Object + 1)

//////////////////////////////////////////////////////////////////////////

EF_STATIC EFInt FindText_FromPos (EFChar* SourceText, EFChar* ToBeFoundText, EFInt nFromPos, EFBool bCaseSensitive);

EFText Integer_IntToString (EFInt Int);
EFText Double_DoubleToString (EFDouble Double);
EFText Boolean_BoolToString (EFBool b);
EFText Binary_BinToString (EFBinary SourceBin);

enum TypeType { INT_TYPE = 0, DOUBLE_TYPE, BOOL_TYPE, TEXT_TYPE, BIN_TYPE, OBJECT_TYPE, INTERFACE_TYPE, ENUM_TYPE };
EFText Array_ToString(EFArray pArray, TypeType type);

inline EFInt sys_Array_GetLength(EFArray array)
{
	return EF_SM_1(CLS_sys_Array, 5, EFInt, EFArray, array);
}

inline EFArray sys_Array_AppendObjectElement(EFArray array, EFObject* element)
{
	return EF_SM_2(CLS_sys_Array, 19, EFArray, EFArray, array, EFObject*, element);
}

inline EFArray sys_Array_InsertObjectElement(EFArray array, EFInt nIdx, EFObject* element)
{
	return EF_SM_3(CLS_sys_Array, 33, EFArray, EFArray, array, EFInt, nIdx, EFObject*, element);
}

inline EFArray sys_Array_DeleteObjectElement(EFArray array, EFInt nFromIndex, EFInt nDeleteCount)
{
	return EF_SM_3(CLS_sys_Array, 47, EFArray, EFArray, array, EFInt, nFromIndex, EFInt, nDeleteCount);
}

#endif //__EF_SYS_BASE_TYPES__
