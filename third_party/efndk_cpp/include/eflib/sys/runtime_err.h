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


#ifndef __EF_SYS_RUNTIME_ERR_H__
#define __EF_SYS_RUNTIME_ERR_H__

// 本文件定义和处理系统运行时错误

#include <ef/types.h>

#define _RPERR_BEGIN_ID  -20001

// 系统定义的运行时错误码
#define RTERR_F_SYS_MALLOC                  (_RPERR_BEGIN_ID - 0)  // 分配内存失败
#define RTERR_F_DIV_ZERO                    (_RPERR_BEGIN_ID - 1)  // 除零
#define RTERR_F_INVALIDATE_EFL_DATA         (_RPERR_BEGIN_ID - 2)  // 无效的类库数据
#define RTERR_F_CREATE_OBJECT_FAILED        (_RPERR_BEGIN_ID - 3)  // 创建对象失败
#define RTERR_F_LOAD_TYPE_INFO_FAILED       (_RPERR_BEGIN_ID - 4)  // 载入类型失败
#define RTERR_F_LIB_NOT_DEFINE_STARTER_CLS  (_RPERR_BEGIN_ID - 5)  // 类库信息中未定义启动类
#define RTERR_F_NOT_FOUND_STARTER_CLS       (_RPERR_BEGIN_ID - 6)  // 欲启动类库未定义启动类
#define RTERR_F_NOT_FOUND_STARTER_METHOD    (_RPERR_BEGIN_ID - 7)  // 未找到或未找到符合要求的启动方法
#define RTERR_F_CREATE_ARY_FAILED           (_RPERR_BEGIN_ID - 8)  // 创建数组失败
#define RTERR_F_FIND_ELEMENT_FAILED         (_RPERR_BEGIN_ID - 9)  // 寻找类型数据成员失败
#define RTERR_F_FIND_METHOD_FAILED          (_RPERR_BEGIN_ID - 10)  // 寻找类方法失败
#define RTERR_F_OBJECT_IS_NULL              (_RPERR_BEGIN_ID - 11)  // 对象为NULL
#define RTERR_F_TYPE_CONVERT_ERR            (_RPERR_BEGIN_ID - 12)  // 转换对象类型失败
#define RTERR_F_ARY_ACCESS_INDEX_ERR        (_RPERR_BEGIN_ID - 13)  // 数组成员索引值无效或超出数组范围
#define RTERR_F_INVALIDATE_ARY_DATA         (_RPERR_BEGIN_ID - 14)  // 无效的数组数据
#define RTERR_F_ASSERT_FAILED               (_RPERR_BEGIN_ID - 15)  // 运行时校验错

void ReportSysRuntimeError (int nSysErrorCode, ...);  // 仅仅用作报告以上的系统定义的运行时错误
// 报告类库自定义运行时错误
void ReportRuntimeError (EFChar* szClsLibName, int nErrorCode, EFChar* szErrorText);
void RemoveErrorHandler ();

#endif //__EF_SYS_RUNTIME_ERR_H__
