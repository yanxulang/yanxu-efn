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


// os.cpp

#include <ef/os.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
	#include <windows.h>
#elif _LINUX
	#include <sys/time.h>
	#include <dlfcn.h>
	#include <unistd.h>
#endif


static EF_OS g_OS_Instance;

EF_OS& EF_OS::GetOS()
{
	return g_OS_Instance;
}

//////////////////////////////////////////////////////////////////////////
// milliseconds() 返回自程序启动到调用时刻的毫秒数。已在Windows和Linux下测试。

#ifdef WIN32
	static EFDWord s_millisecondsWhenProgramBegins = ::GetTickCount();
#else
	static struct timeval s_timevalue = {0};
	static int s_notuse = gettimeofday(&s_timevalue, 0);
	static EFDWord s_millisecondsWhenProgramBegins = s_timevalue.tv_sec*1000 + s_timevalue.tv_usec/1000;
#endif

EFDWord EF_OS::milliseconds()
{
#ifdef WIN32
	EFDWord millisecondsCurrent = ::GetTickCount();
#else
	struct timeval s_timevalue = {0};
	gettimeofday(&s_timevalue, 0);
	EFDWord millisecondsCurrent = s_timevalue.tv_sec*1000 + s_timevalue.tv_usec/1000;
#endif

	if(millisecondsCurrent >= s_millisecondsWhenProgramBegins)
		return (millisecondsCurrent - s_millisecondsWhenProgramBegins);
	else
		return (UINT_MAX - s_millisecondsWhenProgramBegins + millisecondsCurrent);
}

//////////////////////////////////////////////////////////////////////////
// 与用户交互: error(), warning(), information(), yesOrNo()

void EF_OS::error(const EFChar* pText)
{
#ifdef WIN32
	::MessageBox (GetDesktopWindow(), pText, myT("错误："), MB_OK|MB_ICONERROR);
#endif
    EF_printf(myT("\n[错误]：%s\n"), pText);
}

void EF_OS::warning(const EFChar* pText)
{
#ifdef WIN32
	::MessageBox(GetDesktopWindow(), pText, myT("警告："), MB_OK|MB_ICONWARNING);
#elif _LINUX
	EF_printf(myT("\n[警告]：%s\n\007"), pText);
#endif
}

void EF_OS::information(const EFChar* pText)
{
#ifdef WIN32
	::MessageBox(GetDesktopWindow(), pText, myT("信息："), MB_OK);
#elif _LINUX
	EF_printf(myT("\n[信息]：%s\n"), pText);
#endif
}

bool EF_OS::yesOrNo(const EFChar* pText)
{
#ifdef WIN32
	return (::MessageBox(GetDesktopWindow(), pText, myT("询问："), MB_OK|MB_ICONQUESTION|MB_YESNO) == IDYES);
#elif _LINUX
	EF_printf(myT("\n[询问]：%s[Y/N]\n"), pText);
	EFChar c;
	while(c = (EFChar)EF_getchar())
	{
		if(c == myC('Y') || c == myC('y')) return true;
		if(c == myC('N') || c == myC('n')) return false;
	}
#endif
}

//取内存信息, 单位KB
bool EF_OS::GetMemoryInfo(size_t& totalmem, size_t& freemem)
{
	totalmem = freemem = 0;

#ifdef WIN32
	static MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	totalmem = (ms.dwTotalPhys /*+ ms.dwTotalVirtual*/) / 1024;
	freemem  = (ms.dwAvailPhys /*+ ms.dwAvailVirtual*/) / 1024;
	return true;

#elif _LINUX
	static char buffer[1024];
	FILE* pFile = fopen("/proc/meminfo", "r");
	if(pFile == NULL) return false;
	size_t bytes_read = fread(buffer, 1, sizeof(buffer), pFile);
	fclose(pFile);

	if(bytes_read==0 || bytes_read >= sizeof(buffer))
		return false;

	buffer[bytes_read] = '\0';

	char* pos = strstr(buffer, "MemTotal:");
	if(pos == NULL) return false;
	pos += 9; //9: length of "MemTotal:"
	while(*pos != '\0' && (*pos == ' ' || *pos == '\t')) pos++;
	if(*pos == '\0') return false;
	sscanf(pos, "%d", &totalmem);

	pos = strstr(buffer, "MemFree:");
	if(pos == NULL) return false;
	pos += 8; //8: length of "MemFree:"
	while(*pos != '\0' && (*pos == ' ' || *pos == '\t')) pos++;
	if(*pos == '\0') return false;
	sscanf(pos, "%d", &freemem);

	return true;
#endif
}

int EF_OS::GetUsedMemoryPercent()
{
	size_t total, free;
	EF_OS::GetMemoryInfo(total, free);
	return ((total - free) * 100 / total);
}

void _EF_print_last_error_info(const EFChar* szLibraryName)
{
	EF_printf(myT("LoadLibrary: %s\n"), szLibraryName);

	#ifdef _LINUX
		EF_printf(myT("错误信息：%S\n"), dlerror());
	#elif WIN32
		EFDWord dwLastErrorNum = ::GetLastError();

		LPVOID lpMsgBuf = NULL;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, dwLastErrorNum,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL);

		EF_printf(myT("错误码：%d，错误信息：%s"), dwLastErrorNum, lpMsgBuf); 
	 
		LocalFree(lpMsgBuf);
	#endif
}
//////////////////////////////////////////////////////////////////////////
// 加载、卸载动态链接库，取动态库中的函数地址

EFDWord EF_OS::MLoadLibrary (const EFChar* szLibraryName)
{
	EFDWord handle = 0;
#ifdef WIN32
	handle = (EFDWord) ::LoadLibrary (szLibraryName);
#elif _LINUX
	EF_MiniMem memBuffer;
	handle = (EFDWord)dlopen (EF_GetMBSStr ((EFChar*)szLibraryName, memBuffer), RTLD_LAZY);
#endif
	//if(handle == 0) _EF_print_last_error_info(szLibraryName);
	return handle;
}

EFBool EF_OS::MFreeLibrary (EFDWord hLibraryModule)
{
#ifdef WIN32
	return (EFBool) ::FreeLibrary ((HMODULE)hLibraryModule);
#elif _LINUX
	return (EFBool) dlclose ((void*)hLibraryModule);
#endif
}

void* EF_OS::MGetProcAddress (EFDWord hLibraryModule, const char* szProcName) //yes, const char*
{
#ifdef WIN32
	return (void*) ::GetProcAddress ((HMODULE)hLibraryModule, szProcName);
#elif _LINUX
	return (void*) dlsym ((void*)hLibraryModule, szProcName);
#endif
}

void EF_OS::MExit (EFInt nRetCode)
{
#ifdef WIN32
	::TerminateProcess(::GetCurrentProcess(), nRetCode);
#else
	//Linux下调用exit不会引起递归,所以这里不用处理
	exit (nRetCode);
#endif
}

EF_CString EF_OS::NormalizePath(EFChar* szPath)
{
	EF_CString str(szPath);

#ifdef WIN32
	for(EFChar* p = str.GetText(); *p != 0; ++p)
	{
		if(*p >= myC('A') && *p <= myC('Z'))
			*p += (myC('a') - myC('A'));
		if(*p == myC('\\'))
			*p = myC('/');
	}
#endif

	return str;
}

EFBool EF_OS::exec(EFChar* szCommandLine, EFChar* szCurrentDirectory, EFBool bWaitForTerminated, EFBool bHideWindow)
{
#ifdef WIN32
	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	if(bHideWindow)
	{
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	PROCESS_INFORMATION pi;
	if(::CreateProcessW(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, szCurrentDirectory, &si, &pi) == FALSE)
		return EF_FALSE;

	if(bWaitForTerminated)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);
	}
	::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);
	
	return EF_TRUE;
#else
	//TODO:
#endif
}

EF_CString EF_OS::GetFullPath(EFChar* szPath, EFChar* szCurrentDir)
{
	// 取当前目录
	EF_CString strCurrentDir;
	if(szCurrentDir == NULL)
	{
		const EFInt BUF_SIZE = 1024;
		EFChar CurrentDirBuf[BUF_SIZE] = {0};
#ifdef WIN32
		::GetCurrentDirectoryW(BUF_SIZE, CurrentDirBuf);
		strCurrentDir = CurrentDirBuf;
#elif _LINUX
		getcwd((char*)CurrentDirBuf, BUF_SIZE * sizeof(EFChar));
		strCurrentDir.SetText((char*)CurrentDirBuf);
#endif
	}
	else
	{
		strCurrentDir = szCurrentDir;
	}

	// 按'/'或'\\'分割
	EF_CStringArray tokens;
	EF_CString strPath = strCurrentDir;
	strPath += myT("/");
	strPath += szPath;
	EFChar* szPrevToken = strPath.GetText();
	for(EFChar* p = szPrevToken; *p; ++p)
	{
		if(*p == myC('\\') || *p == myC('/'))
		{
			tokens.Add(szPrevToken, p - szPrevToken);
			szPrevToken = p + 1;
		}
	}
	if(*szPrevToken)
	{
		tokens.Add(szPrevToken);
	}

	// 处理 "." 和 ".."
	EF_CStringArray tempArray;
	for(EFInt n = 0; n < tokens.GetCount(); ++n)
	{
		EFChar* szTok = tokens[n];
		if(*szTok == 0)
			continue;
		else if(EF_strcmp(myT("."), szTok) == 0)
			continue;
		else if(EF_strcmp(myT(".."), szTok) == 0)
		{
			EFInt nTempArrayCount = tempArray.GetCount();
			ASSERT(nTempArrayCount > 1);
			if(nTempArrayCount > 1)
			{
				tempArray.RemoveAt(nTempArrayCount- 1);
			}
		}
		else
		{
			tempArray.Add(szTok);
		}
	}
	
	// 生成最终文本
	EFInt nCount =  tempArray.GetCount();
	EF_CString strReturn;
	for(EFInt n2 = 0; n2 <nCount; ++n2)
	{
		strReturn += tempArray[n2];
		if(n2 != nCount - 1)
		{
			strReturn += myT("/");
		}
	}

	return strReturn;

}

EF_CString EF_OS::GetProcessImageFilePath(void)
{
#ifdef WIN32
		const DWORD buffer_size = 1024;
		EFChar buffer[buffer_size];
		GetModuleFileName(NULL, buffer, buffer_size-1);
		return buffer;
#elif _LINUX
		const static char* PROC_SELF_EXE = "/proc/self/exe";
		int nBufSize = 1024;
		char* pBuf = (char*)EF_malloc(nBufSize);
		int nReadSize = readlink(PROC_SELF_EXE, pBuf, nBufSize);
		if(nReadSize > 0)
		{
			while(nReadSize >= nBufSize)
			{
				nBufSize *= 2;
				EF_free(pBuf);
				pBuf = (char*)EF_malloc(nBufSize);
				nReadSize = readlink(PROC_SELF_EXE, pBuf, nBufSize);
			}
			pBuf[nReadSize] = '\0';
			EF_CString str;
			str.SetText(pBuf);
			EF_free(pBuf);
			return str;
		}
		EF_free(pBuf);
		return myT("");
#endif
}
