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


//sys.h

#ifndef __EF_SYS_H__
#define __EF_SYS_H__

#define EF_LIB_NAME_SYS  myT("系统")

#include <ef/ef.h>
#include <ef/utils.h>
#include "object.h"
#include "runtime.h"
#include "event.h"


EFObject* EF_CreateObject (EFChar* szClassName, EFBool blCallDefaultConstructor);
// 处理事件的发送过程。具体的事件处理（包括是否处理）交由 pfnOnNeedDoEvent() 来完成。
typedef void (*EF_PFN_OnNeedDoEvent) (EFObject* pProcessorObject, EFObject* pEventObject, 
                                      EFMethodInfo* pMethod, EFBool* pblNeedProcess, EFBool* pblIsContinue);
EFBool EF_SendEvent (EFObject* pProcessorObject, EFObject* pEventObject, EF_PFN_OnNeedDoEvent pfnOnNeedDoEvent);

// 启动调试类型载入通知的固定命令行参数
#define EF_START_DEBUGGER_NOTIFY_ARG_W  myT ("--@Debugging")
#define EF_START_DEBUGGER_NOTIFY_ARG_A  "--@Debugging"


typedef enum
{
    EF_NDC_NOTIFY_TYPE_LOADER,
    EF_NDC_TYPE_LOADED,
    EF_NDC_RUNTIME_ERROR,
	EF_NDC_NOTIFY_CF_FUNC_TABLE = 0x10000000,
	EF_NDC_FUNCTION_RETURNED,
	EF_NDC_DBG_LIB_UNLOADING,
	EF_NDC_LOADINTERFACE,
}
EF_NOTIFY_DEBUGGER_CODE;

/*
nNotifyCode: EF_NDC_NOTIFY_TYPE_LOADER
    通知类型加载器。dwParam1：GDebuggerLoadTypeInfo函数地址； dwParam2：s_szLoadTypeName内存地址
nNotifyCode: EF_NDC_TYPE_LOADED
    类型加载通知。dwParam1：所加载类型的EFTypeInfo*指针。
nNotifyCode: EF_NDC_RUNTIME_ERROR
    运行时错误通知。dwParam1：运行时错误码； dwParam2：运行时错误文本信息
*/
void EF_NotifyDebugger (EF_NOTIFY_DEBUGGER_CODE enNotifyCode, EFDWord dwParam1 = 0, EFDWord dwParam2 = 0);


#endif //__EF_SYS_H__
