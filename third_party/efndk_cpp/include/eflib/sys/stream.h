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


//stream.h

#ifndef __EF_SYS_STREAM_H__
#define __EF_SYS_STREAM_H__

#include "sys.h"

#define I_sys_IStream  myT("系统.输入流")
#define I_sys_OStream  myT("系统.输出流")

///////////////////////////////////////////////////////////////////////////
// IStream 

inline EFInt sys_IStream_ReadInt (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 0, EFInt);
}

inline EFDouble sys_IStream_ReadDouble (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 1, EFDouble);
}

inline EFBool sys_IStream_ReadBool (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 2, EFBool);
}

inline EFText sys_IStream_ReadText (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 3, EFText);
}

inline EFBinary sys_IStream_ReadBin (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 4, EFBinary);
}

inline EFObject* sys_IStream_ReadObject (EFInterface* pThis)
{
	return EF_IM_0 (pThis, 5, EFObject*);
}


///////////////////////////////////////////////////////////////////////////
// OStream 

inline EFBool sys_OStream_WriteInt (EFInterface* pThis, EFInt value)
{
	return EF_IM_1 (pThis, 0, EFBool, EFInt, value);
}

inline EFBool sys_OStream_WriteDouble (EFInterface* pThis, EFDouble value)
{
	return EF_IM_1 (pThis, 1, EFBool, EFDouble, value);
}

inline EFBool sys_OStream_WriteBool (EFInterface* pThis, EFBool value)
{
	return EF_IM_1 (pThis, 2, EFBool, EFBool, value);
}

inline EFBool sys_OStream_WriteText (EFInterface* pThis, EFText value)
{
	return EF_IM_1 (pThis, 3, EFBool, EFText, value);
}

inline EFBool sys_OStream_WriteBin (EFInterface* pThis, EFBinary value)
{
	return EF_IM_1 (pThis, 4, EFBool, EFBinary, value);
}

inline EFBool sys_OStream_WriteObject (EFInterface* pThis, EFObject* value)
{
	return EF_IM_1 (pThis, 4, EFBool, EFObject*, value);
}


#endif //__EF_SYS_STREAM_H__
