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


//event.h

#ifndef __EF_SYS_EVENT_H__
#define __EF_SYS_EVENT_H__

#define CLS_sys_Event  myT("系统.事件")
const int IDX_sys_Event = 1;

#define sys_Event_constructor(pThis, hSenderObject)		EF_CM_1(1, 0, void, pThis, EFObject*, pSenderObject)
#define sys_Event_GetSender(pThis)						EF_CM_0(1, 1, EFObject*, pThis)
#define sys_Event_SetSender(pThis, pSenderObject)       EF_CM_1(1, 2, void, pThis, EFObject*, pSenderObject)
#define sys_Event_GetUserObject(pThis)					EF_CM_0(1, 3, EFObject*, pThis)
#define sys_Event_SetUserObject(pThis, pSenderObject)   EF_CM_1(1, 4, void, pThis, EFObject*, pSenderObject)

#define _sys_Event_constructor(pThis, pSenderObject)	EF_OM_1(1, 0, void, pThis, EFObject*, pSenderObject)
#define _sys_Event_GetSender(pThis)						EF_OM_0(1, 1, EFObject*, pThis)
#define _sys_Event_SetSender(pThis, pSenderObject)      EF_OM_1(1, 2, void, pThis, EFObject*, pSenderObject)
#define _sys_Event_GetUserObject(pThis)					EF_OM_0(1, 3, EFObject*, pThis)
#define _sys_Event_SetUserObject(pThis, pSenderObject)  EF_OM_1(1, 4, void, pThis, EFObject*, pSenderObject)

#endif //__EF_SYS_EVENT_H__
