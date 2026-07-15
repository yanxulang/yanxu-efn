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


// ecl_utils.cpp

#include <stdlib.h>
#include <string.h>

#include <ef/utils.h>
#include <ef/os.h>

//////////////////////////////////////////////////////////////////////////

//枚举指定的所有方法(连同其基类方法)
void EF_EnumObjectMethods(EFObject* pObject, EF_PFN_EnumObjectMethods pEnumFunction, void* pUserData)
{
	ASSERT(pEnumFunction);
	if(pEnumFunction == NULL) return;

	EFInt nClassCount = pObject->m_pObjectInfo->m_nClassCount;
	EFSingleObjectRuntimeInfo* pRuntimeInfo = &(pObject->m_pObjectInfo->m_aryInfos[nClassCount-1]);
	
	EFClassInfo* pClassInfo = NULL;
	for(EFInt nClassIndex = nClassCount - 1; nClassIndex >= 0; nClassIndex--, pRuntimeInfo--)
	{
		pClassInfo = pRuntimeInfo->m_pClassInfo;

		if(pClassInfo->m_pFieldTable == NULL) continue;
		for(EFInt nMethodIndex = 0; nMethodIndex < pClassInfo->m_nMethodCount; nMethodIndex++)
		{
			pEnumFunction(&(pClassInfo->m_pMethodTable[nMethodIndex]), pUserData);
		}
	}
}

void EF_EnumObjectFields(EFObject* pObject, EF_PFN_EnumObjectFields pEnumFunction, void* pUserData)
{
	ASSERT(pEnumFunction);
	if(pEnumFunction == NULL) return;

	EFInt nClassCount = pObject->m_pObjectInfo->m_nClassCount;
	EFSingleObjectRuntimeInfo* pRuntimeInfo = &(pObject->m_pObjectInfo->m_aryInfos[nClassCount-1]);

	EFClassInfo* pClassInfo = NULL;
	for(EFInt nClassIndex = nClassCount - 1; nClassIndex >= 0; nClassIndex--, pRuntimeInfo--)
	{
		pClassInfo = pRuntimeInfo->m_pClassInfo;

		if(pClassInfo->m_pFieldTable == NULL) continue;
		for(EFInt nElementIndex = 0; nElementIndex < pClassInfo->m_nFieldCount; nElementIndex++)
		{
			pEnumFunction(&(pClassInfo->m_pFieldTable[nElementIndex]), pUserData);
			//TODO: 把数据存放地址也传入枚举函数
		}
	}
}

////////////////////////////////////////////////////////////////////

EFBool EF_IsGetterMethod(EFMethodInfo* pMethod)
{
	return  pMethod 
		&& (pMethod->m_dwState & EF_S_M_PROPERTY) //属性标记
		&& (pMethod->m_nArgCount == 0) //无参数
		&& (EF_strlen(pMethod->m_szReturnDataType) > 0); //有返回值
}

EFBool EF_IsSetterMethod(EFMethodInfo* pMethod)
{
	return  pMethod 
		&& (pMethod->m_dwState & EF_S_M_PROPERTY) //属性标记
		&& (pMethod->m_nArgCount == 1) //一个参数
		&& (EF_strlen(pMethod->m_szReturnDataType) == 0); //无返回值
}

EFBool EF_IsPropertyMethod(EFMethodInfo* pMethod)
{
	return EF_IsGetterMethod(pMethod) || EF_IsSetterMethod(pMethod);
}

//////////////////////////////////////////////////////////////////////////

EF_Mallocer::EF_Mallocer() : m_pMem(NULL), m_nSize(0)
{
}

EF_Mallocer::EF_Mallocer(int nMemSize)
{
	ASSERT(nMemSize > 0);
	m_nSize = nMemSize;
	m_pMem  = EF_malloc(nMemSize);
}

EF_Mallocer::~EF_Mallocer()
{
	if(m_pMem) EF_free(m_pMem);
}

void EF_Mallocer::Atach(void* pMem, int nMemSize)
{
	if(m_pMem) EF_free(m_pMem);

	m_pMem  = pMem;
	m_nSize = nMemSize;
}

void* EF_Mallocer::Detach() 
{
	void* pMem = m_pMem;
	m_pMem  = NULL;
	m_nSize = 0;
	return pMem;
}

int EF_Mallocer::GetSize()     { return m_nSize; }

void* EF_Mallocer::GetData()   { return m_pMem; }

EF_Mallocer::operator void* () { return m_pMem; }


EF_EFDataWrapper::EF_EFDataWrapper() : m_pData(NULL)
{
}

EF_EFDataWrapper::EF_EFDataWrapper(void* pData) : m_pData(pData)
{
	m_pData = pData;
}

EF_EFDataWrapper::~EF_EFDataWrapper()
{
	if(m_pData)
		EF_DEC_REF_COUNT(m_pData);
}

void EF_EFDataWrapper::Atach(void* pData)
{
	if(m_pData != NULL)
		EF_DEC_REF_COUNT(m_pData);

	ASSERT(pData);
	m_pData = pData;
}

void* EF_EFDataWrapper::Detach()
{
	void* pData = m_pData;
	m_pData = NULL;
	return pData;
}

void* EF_EFDataWrapper::AddRef()
{
	return EF_INC_REF_COUNT(m_pData);
}

void* EF_EFDataWrapper::GetData()
{
	return m_pData;
}


EF_ObjectWrapper::EF_ObjectWrapper(EFChar* szClassName)
{
	ASSERT(szClassName); 
	m_pData = (void*) EF_NEW_OBJECT(szClassName);
	ASSERT(m_pData);
}

EF_ObjectWrapper::operator EFObject* () { return (EFObject*) GetData(); }


EF_TextWrapper::EF_TextWrapper(EFChar* szText)
{
	ASSERT(szText);
	m_pData = (void*) EF_NEW_TEXT(szText);
}

EF_TextWrapper::EF_TextWrapper(EFText etext)
{
	m_pData = etext;
}

EFChar* EF_TextWrapper::GetText()
{
	if(m_pData == NULL) 
		return NULL;
	else
		return EF_GET_TEXT(m_pData);
}

EF_TextWrapper::operator EFText () { return (EFText) GetData(); }

EF_TextWrapper::operator EFChar* () { return GetText(); }


EF_BinWrapper::EF_BinWrapper(int nLength) //malloc new mem, and reg it to gc.
{
	EF_Mallocer mallocer(nLength + 2 * sizeof(EFInt));
	void* pEBin = mallocer.Detach();

	EFInt* pEBinHeader = (EFInt*)pEBin;
	//pEBinHeader[0] will be set by EF_GC_REG_BIN_DATA
	pEBinHeader[1] = 1;
	pEBinHeader[2] = nLength;

	m_pData = (void*) EF_GC_REG_BIN_DATA(pEBin);
}

EF_BinWrapper::EF_BinWrapper(void* pMem, int nLength) //copy data from pMem, and reg copyed data to gc.
{
	ASSERT(pMem && nLength > 0);
	m_pData = (void*) EF_NEW_BIN(pMem, nLength);
}

EF_BinWrapper::operator EFBinary () { return (EFBinary) GetData(); }

void* EF_BinWrapper::GetCData(int* pLength/* = NULL*/)
{
	int nLen = 0;
	void* pBin = EF_GetEFBinDataAndLength((EFBinary)GetData(), &nLen);
	if(pLength) *pLength = nLen;
	return pBin;
}

int EF_BinWrapper::GetCDataLength()
{
	int nLen;
	EF_GetEFBinDataAndLength((EFBinary)GetData(), &nLen);
	return nLen;
}


//////////////////////////////////////////////////////////////////////////

EFSysLibLoader::EFSysLibLoader()
{
	m_SysLibHandle = 0;
	m_pRuntimeObject = NULL;

	EF_CStringArray paths;
	EF_CString syslibpath;

	EF_GetEFLibSearchPaths(paths);
	int nCount = paths.GetCount (), i;
	for (i = 0; i < nCount; i++)
	{
		syslibpath = paths [i];
		syslibpath += myT("系统"); //!直接使用了系统库名称"系统"
		syslibpath += EF_EFN_FILE_EXT;

		m_SysLibHandle = EF_OS::MLoadLibrary(syslibpath);

		if (m_SysLibHandle != 0) 
			break;
	}

	if(m_SysLibHandle == 0) 
	{
		EF_OS::error(myT("无法加载“易语言.飞扬”系统类库！\n\n")
				  myT("请检查类库文件“系统.efn”是否存在，或系统环境变量“")
				  EF_LIB_PATHS_NAME_W
				  myT("”是否已正确设置。"));
		return;
	}

	//获取系统库导出函数EInitSysLib()的执行地址并调用之，以获取pRuntime。
	typedef EFObject* (*PFN_InitSysLib) (void);
	PFN_InitSysLib pfn_InitSysLib = (PFN_InitSysLib) EF_OS::MGetProcAddress(m_SysLibHandle, "EF_InitSysLib"); //!直接使用了DLL导出函数名称"EF_InitSysLib"
	ASSERT(pfn_InitSysLib);
	if(pfn_InitSysLib)
		m_pRuntimeObject = pfn_InitSysLib(); //初始化系统库，获取Runtime对象指针
}

EFSysLibLoader::~EFSysLibLoader()
{
	if(m_SysLibHandle)
		EF_OS::MFreeLibrary(m_SysLibHandle);
}

EFObject* EFSysLibLoader::GetRuntimeObject()
{
	ASSERT(m_pRuntimeObject);
	return m_pRuntimeObject;
}

EFLibInfo* EFSysLibLoader::GetLibInfo()
{
	if(m_pRuntimeObject)
		return EF_GET_CLASS_INFO(m_pRuntimeObject)->m_pLibInfo;
	
	ASSERT(EF_FALSE);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//szz0SeparatedString文本格式：以myC('\0')分隔的多个文本，最后以myC('\0')结束
EFArray EF_GetTextArrayOf0SeparatedString(EFChar* szz0SeparatedString)
{
	EF_IntArray ary;
	ary.Add(0);
	ary.Add(1);
	ary.Add(0);

	if(szz0SeparatedString == NULL || szz0SeparatedString[0] == myC('\0'))
		return EF_GC_REG_TEXT_OR_BIN_ARRAY_DATA(ary.Detach());

	EFChar* p = szz0SeparatedString;
	int count = 0;
	while(1)
	{
		ary.Add((EFInt)(intptr_t)EF_NEW_TEXT(p));
		count++;
		p += (EF_strlen(p) + 1);
		if(*p == myC('\0')) break;
	}

	ary[2] = count;
	return EF_GC_REG_TEXT_OR_BIN_ARRAY_DATA(ary.Detach());
}
