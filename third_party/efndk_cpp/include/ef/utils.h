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


// ecl_utils.h
#ifndef __EF_UTILS_H__
#define __EF_UTILS_H__

#include <stdarg.h>
#include <ef/ef.h>

/*
//即将删除, 如果其它工程不能编译, 请及时将代码复制到合适的地方

#define M_MAKEINT(a, b)      ((int)(((unsigned short)((unsigned int)(a) & 0xffff)) | \
		((unsigned int)((unsigned short)((unsigned int)(b) & 0xffff))) << 16))
#define M_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)((unsigned int)(a) & 0xff)) | ((unsigned short)((unsigned char)((unsigned int)(b) & 0xff))) << 8))
#define M_LOWORD(l)	 ((unsigned short)((unsigned int)(l) & 0xffff))
#define M_HIWORD(l)  ((unsinged short)((unsigned int)(l) >> 16)) 	
#define M_LOBYTE(w)           ((unsigned char)((unsigned int)(w) & 0xff))
#define M_HIBYTE(w)           ((unsigned char)((unsigned int)(w) >> 8))
*/

EFBool EF_IsGetterMethod (EFMethodInfo *pItem);
EFBool EF_IsSetterMethod (EFMethodInfo *pItem);
EFBool EF_IsPropertyMethod(EFMethodInfo* pMethodInfo);


typedef void (*EF_PFN_EnumObjectMethods) (EFMethodInfo* pMethodInfo, void* pUserData);
typedef void (*EF_PFN_EnumObjectFields) (EFFieldInfo* pFieldInfo, void* pUserData);

void EF_EnumObjectMethods(EFObject* pObject, EF_PFN_EnumObjectMethods pEnumFunction, void* pUserData);
void EF_EnumObjectFields(EFObject* pObject, EF_PFN_EnumObjectFields pEnumFunction, void* pUserData);


EFArray EF_GetTextArrayOf0SeparatedString(EFChar* szz0SeparatedString);

//////////////////////////////////////////////////////////////////////////

// EF_Mallocer, EF_ObjectWrapper, EF_TextWrapper, EF_BinWrapper, EF_ArrayWrapper: 
// 在其构造函数中分配内存/创建对象，在其析构函数中释放内存/销毁对象（减小引用计数）。
// 使用以上对象，即使在发生异常的情况下，也可确保内存或对象被释放或销毁。


//负责分配内存，并在析构时释放该内存
class EF_Mallocer
{
public:
	EF_Mallocer();
	EF_Mallocer(int nMemSize);
	~EF_Mallocer();
	void Atach(void* pMem, int nMemSize);
	void* Detach();
	int GetSize();
	void* GetData();
	operator void* ();

private:
	void* m_pMem;
	int   m_nSize;
};


class EF_EFDataWrapper
{
public:
	EF_EFDataWrapper();
	EF_EFDataWrapper(void* pData);
	virtual ~EF_EFDataWrapper();
	virtual void Atach(void* pData);
	virtual void* Detach();
	virtual void* AddRef();
	void* GetData();
protected:
	void* m_pData;
};


//负责创建易语言对象，并在析构时将该对象引用计数减一
class EF_ObjectWrapper : public EF_EFDataWrapper
{
public:
	EF_ObjectWrapper(EFChar* szClassName);
	operator EFObject* ();
};


//负责创建易语言文本对象，并在析构时将该对象引用计数减一
class EF_TextWrapper : public EF_EFDataWrapper
{
public:
	EF_TextWrapper(EFChar* szText);
	EF_TextWrapper(EFText etext);
	EFChar* GetText();
	operator EFText ();
	operator EFChar* ();
};

//负责创建易语言字节集对象，并在析构时将该对象引用计数减一
class EF_BinWrapper : public EF_EFDataWrapper
{
public:
	EF_BinWrapper(int nLength); //malloc new mem, and reg it to gc.
	EF_BinWrapper(void* pMem, int nLength); //copy data from pMem, and reg copyed data to gc.
	operator EFBinary ();
	void* GetCData(int* pLength = NULL);
	int GetCDataLength();
};

class EF_ArrayWrapper : public EF_EFDataWrapper
{
	//todo:
};


//////////////////////////////////////////////////////////////////////////

//负责 EFInterface* 数据的创建、填充和释放
class EF_InterfaceWrapper
{
private:
	EFInterface* m_pInterfaceData;

public:
	EF_InterfaceWrapper(EFChar* szInterfaceName, EFObject* pObject, int nInterfaceNameHash=0);
	EF_InterfaceWrapper(const EF_InterfaceWrapper& other);
	~EF_InterfaceWrapper();
	EFInterface* GetInterfaceData() const;
	operator EFInterface* () const;

};

//////////////////////////////////////////////////////////////////////////

//通常用于C++写的“易语言类库测试程序”中，负责加载并初始化核心库，并获取Runtime和LibInfo
class EFSysLibLoader
{
public:
	EFSysLibLoader();
	~EFSysLibLoader();
	EFObject*  GetRuntimeObject();
	EFLibInfo* GetLibInfo();
private:
	EFDWord   m_SysLibHandle;
	EFObject* m_pRuntimeObject;
};

//////////////////////////////////////////////////////////////////////////

#endif //__EF_UTILS_H__
