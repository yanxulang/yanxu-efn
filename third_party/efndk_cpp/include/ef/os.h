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


// os.h
// 本文件用于封装与操作系统有关的一些跨平台代码

#ifndef __EF_OS_H__
#define __EF_OS_H__

#include <ef/ef.h>

class EF_OS
{
public:
	static EF_OS& GetOS();
public:
	//返回自程序启动以来的毫秒数
	static EFDWord milliseconds();

	//与用户交互函数
	static void error(const EFChar* pText);
	static void warning(const EFChar* pText);
	static void information(const EFChar* pText);
	static bool yesOrNo(const EFChar* pText);

	//取内存信息, 单位KB
	static bool GetMemoryInfo(size_t& totalmem, size_t& freemem);
	static int  GetUsedMemoryPercent();
	// 规范文件名的大小写(windows下一律转换到小写，linux下保持不变)
	static EF_CString NormalizePath(EFChar* szPath);
	static EF_CString GetFullPath(EFChar* szPath, EFChar* szCurrentDir = NULL);

    	static EFDWord MLoadLibrary (const EFChar* szLibraryName);
    	static EFBool MFreeLibrary (EFDWord hLibraryModule);
    	static void* MGetProcAddress (EFDWord hLibraryModule, const char* szProcName);
   	static void MExit(EFInt nRetCode);

	static EFBool exec(EFChar* szCommandLine, EFChar* szCurrentDirectory, EFBool bWaitForTerminated, EFBool bHideWindow);
	// 取得当前进程的可执行文件全路径	
	static EF_CString GetProcessImageFilePath(void);
};

#endif //__EF_OS_H__
