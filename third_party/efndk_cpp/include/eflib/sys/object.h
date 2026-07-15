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


//object.h

#ifndef __EF_SYS_OBJECT_H__
#define __EF_SYS_OBJECT_H__


#define CLS_sys_Object          myT ("系统.对象")
#define I_sys_BaseInterface     myT ("系统.接口")
#define IDX_sys_Object 0


#define sys_Object_IsTypeSame(pThis, pOtherObject)                     EF_CM_1 (0, 0, EFBool, pThis, EFObject*, pOtherObject)
#define sys_Object_IsInstanceOf(pThis, szClassName)                    EF_CM_1 (0, 1, EFBool, pThis, EFText, szClassName)
#define sys_Object_GetClassName(pThis)                                 EF_CM_0 (0, 2, EFText, pThis)
#define sys_Object_SendEvent(pThis, pEventObject)                      EF_CM_1 (0, 3, EFBool, pThis, EFObject*, pEventObject)
#define sys_Object_SendEvent2(pThis, pEventObject, pfnOnNeedDoEvent)   EF_CM_2 (0, 4, EFBool, pThis, EFObject*, pEventObject, EF_PFN_OnNeedDoEvent, pfnOnNeedDoEvent)
#define sys_Object_IsInstanceOf2(pThis, szClassName)                   EF_CM_1 (0, 5, EFBool, pThis, EFChar *, szClassName)
#define sys_Object_Equals(pThis, pOtherObject)                         EF_CM_1 (0, 6, EFBool, pThis, EFObject*, pOtherObject)
#define sys_Object_CompareTo(pThis, pOtherObject)                      EF_CM_1 (0, 7, EFBool, pThis, EFObject*, pOtherObject)
#define sys_Object_ToString(pThis)                                     EF_CM_0 (0, 8, EFText, pThis)

#define _sys_Object_IsTypeSame(pThis, pOtherObject)                    EF_OM_1 (0, 0, EFBool, pThis, EFObject*, pOtherObject)
#define _sys_Object_IsInstanceOf(pThis, szClassName)                   EF_OM_1 (0, 1, EFBool, pThis, EFText, szClassName)
#define _sys_Object_GetClassName(pThis)                                EF_OM_0 (0, 2, EFText, pThis)
#define _sys_Object_SendEvent(pThis, pEventObject)                     EF_OM_1 (0, 3, EFBool, pThis, EFObject*, pEventObject)
#define _sys_Object_SendEvent2(pThis, pEventObject, pfnOnNeedDoEvent)  EF_OM_2 (0, 4, EFBool, pThis, EFObject*, pEventObject, EF_PFN_OnNeedDoEvent, pfnOnNeedDoEvent)
#define _sys_Object_IsInstanceOf2(pThis, szClassName)                  EF_OM_1 (0, 5, EFBool, pThis, EFChar *, szClassName)
#define _sys_Object_Equals(pThis, pOtherObject)                        EF_OM_1 (0, 6, EFBool, pThis, EFObject*, pOtherObject)
#define _sys_Object_CompareTo(pThis, pOtherObject)                     EF_OM_1 (0, 7, EFBool, pThis, EFObject*, pOtherObject)
#define _sys_Object_ToString(pThis)                                    EF_OM_0 (0, 8, EFText, pThis)

#endif //__EF_SYS_OBJECT_H__
