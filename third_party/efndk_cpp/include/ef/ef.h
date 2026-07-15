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


#ifndef __EF_H__
#define __EF_H__

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <ef/types.h>
#include <ef/hashmap.h>
#include <ef/functions.h>


////////////////////////////////////////////////////////////////////////////////

// EFL, EF Library
#define EF_EFL_FILE_EXT  myT(".efl")  // 使用“易语言.飞扬”自身开发的EF类库文件名后缀

// EFN, EF Native library
#define EF_EFN_FILE_EXT  myT(".efn")  // 使用C++等其它编程语言开发的的EF类库文件名后缀

// 所有EF本地类库都必须导出的函数名称及其原型
#define EF_GET_LIB_INFO_FUNC_EXPORT_NAME    "EF_GetLibInfo"
#define EF_GET_LIB_INFO_FUNC_EXPORT_NAME_W  myT("EF_GetLibInfo")

struct EFLibInfo;
typedef EFLibInfo* (*EF_PFN_GET_LIB_INFO) ();

// 指定EF类库加载路径的系统环境变量名称
#define EF_LIB_PATHS_NAME    "EF_LIB_PATHS"
#define EF_LIB_PATHS_NAME_W  myT("EF_LIB_PATHS")

////////////////////////////////////////////////////////////////////////////////

typedef struct _tagEFVirtualTableItem
{
    // 记录当前类方法进行了虚拟覆盖处理后的方法地址，因此本方法地址可能指向其继承类中的覆盖方法地址。
    EF_PFUNC m_pfnCurrent;
    // 记录当前类方法定义时所提供的方法地址
    EF_PFUNC m_pfnOld;
}
EFVirtualTableItem;

struct EFClassInfo;

typedef struct _tagEFSingleObjectRuntimeInfo
{
    EFClassInfo* m_pClassInfo;
    // 所指向的是一个EFVirtualTableItem数组，顺序记录m_pClassInfo和其实现的接口中定义的所有虚函数（非私有/非静态）方法地址，
	// 记录顺序为：类自身的方法，类所实现接口的方法。
    EFVirtualTableItem* m_pVirtualFuncTable;
}
EFSingleObjectRuntimeInfo;

typedef struct _tagEFObjectRuntimeInfo
{
    EFInt m_nClassCount;
    // 下面顺序记录从最基础类到当前对象类的信息。
    EFSingleObjectRuntimeInfo m_aryInfos [1 /*m_nClassCount*/];
}
EFObjectRuntimeInfo;

struct EFObject
{
    EFDWord m_dwRefCount;  // 本对象的被引用计数和类型
    EFObjectRuntimeInfo* m_pObjectInfo;  // 本对象的运行时类型信息
    // 下面顺序记录从最基础类到当前对象类的类数据。
    void* m_paryData [1 /*m_pObjectInfo->m_nClassCount*/];
};
typedef struct EFObject EFObject;

struct EFInterfaceInfo;

typedef struct _tagEFInterface
{
    EFDWord m_dwRefCount;  // 本接口对象的被引用计数和类型
    EFObject* m_pObject;   // 本接口所参考到的对象数据
    EFVirtualTableItem* m_pInterfaceFuncs;  //   该接口的实现方法地址表，注意是从其最基础接口方法表开始顺序记录。
                                            // 注意EFVirtualTableItem中的m_pfnOld在此没有意义。
    EFInterfaceInfo* m_pInterfaceInfo;      // 接口定义信息
}
EFInterface;

/*
对象数据格式实例剖析：

    假设存在以下类和接口定义:

         接口 接口1
         {
             接口1_方法1 ();
             接口1_方法2 ();
         }

         接口 接口2 <基接口 = "接口1">
         {
             接口2_方法1 ();
         }

         接口 接口3
         {
             接口3_方法1 ();
         }

         类 类1 <接口实现表 = " 接口3, 接口2">
         {
             接口1_方法1 ()  { }
             接口1_方法2 ()  { }
             接口3_方法1 ()  { }
             接口2_方法1 ()  { }

             类1_方法1 ()  { }
             方法2 ()  { }

             整数 类1_数据1;
             整数 类1_数据2;
         }

         类 类2 <基类 = "类1">
         {
             类2_方法1 ()  { }
             方法2 ()  { }

             整数 类2_数据1;
         }         

    那么，假设存在语句:

        类2 abc = 创建 类2;

    此语句执行后 abc 对象数据 EFObject 结构的各成员值具体为：

        m_dwRefCount   = 1               // 新创建出来对象实例的 m_nRefCount 将被初始化为 1
        m_pObjectInfo = 类2_pObjectInfo  // 指向类2的运行时信息
        m_paryData =
            类1_pData,  // 指向对象中类1部分的数据
            类2_pData   // 指向对象中类2部分的数据

    类1_pData 指针所指向的数据区域为两个整数，分别为“类1_数据1”和“类1_数据2”的具体运行时值。
    类2_pData 指针所指向的数据区域为一个整数，为“类2_数据1”的具体运行时值。

    类2_pObjectInfo 所指向的是一个 EFObjectRuntimeInfo 结构，其各成员值具体为：
    
        m_nClassCount = 2   // 包括基础类在内共两个类（在此忽略掉系统默认的所有类共有最基础类）
        m_aryInfos =
            类1_pSingleObjectRuntimeInfo,  // 指向类1的运行时定义数据
            类2_pSingleObjectRuntimeInfo   // 指向类2的运行时定义数据

    类1_pSingleObjectRuntimeInfo 所指向的是一个 EFSingleObjectRuntimeInfo 结构，其各成员值具体为：
    
        m_pClassInfo 指向“类1”的类定义信息。
        m_pVirtualFuncTable 指向的数据区顺序记录类1的所有非私有和静态成员方法代码地址，具体为：

        // 首先记录类本身不在接口中的成员方法：

            { 类1_方法1,  类1_方法1 },
            { 方法2（由于本方法被类2覆盖，所以它指向类2中“方法2”实现代码地址）,  方法2（记录类1中本来的“方法2”实现代码地址） },

        // 然后顺序记录类所实现的每个接口的成员方法，如果某个接口具有基接口，先记录其基接口的成员方法：

            { 接口3_方法1, 接口3_方法1 },

            { 接口1_方法1, 接口1_方法1 },
            { 接口1_方法2, 接口1_方法2 },
            { 接口2_方法1, 接口2_方法1 },

    类2_pSingleObjectRuntimeInfo 所指向的是一个 EFSingleObjectRuntimeInfo 结构，其各成员值具体为：

        m_pClassInfo 指向“类2”的类定义信息。
        m_pVirtualFuncTable 顺序记录类2的所有非私有和静态成员方法代码地址，具体为：

            { 类2_方法1,  类2_方法1 },
            { 方法2（类2中“方法2”实现代码地址，后同）,  方法2 },
*/

// nIndex: 0、1、2 ..... ，按照从当前类到其最基础类的顺序。
#define EF_GET_SPEC_CLASS_INFO(pObject, nIndex)  \
	(((EFObject*)pObject)->m_pObjectInfo->m_aryInfos [((EFObject*)pObject)->m_pObjectInfo->m_nClassCount - 1 - nIndex].m_pClassInfo)

#define EF_GET_CLASS_INFO_BY_INDEX(pObject, nClassIndex)  \
	(((EFObject*)pObject)->m_pObjectInfo->m_aryInfos [nClassIndex].m_pClassInfo)

#define EF_GET_CLASS_INFO(pObject)  \
	EF_GET_SPEC_CLASS_INFO(pObject, 0)

////////////////////////////////////////////////////////////////////////////////

struct EFLibInfo;

/*
扩展属性表m_szExtendedPropertyList说明:
    1、属性表格式： 为类似"属性名 属性名=属性值 ..."的文本, 各扩展属性之间用空格分开。
    2、属性值可以为文本、逻辑值、数值。
    3、数值型属性值可以为二进制(0B开头)、十六进制(0X开头)、十进制数值，比如：
         长度 = 0x123
         长度 = 0b10101
         长度 = 123
    4、文本型属性值用双引号括住，中可以包含转义符，具体支持的转义符及其含义如下：
        \t          制表符
        \r          回车符
        \n          换行符
        \\          \
        \"          "
        \'          '
        \x????      ?为十六进制字符，用作具体提供某个字符的十六进制码值
    5、可以省略文本型属性值两端的双引号，但此时有以下要求：
        A、文本值必须为一个有效的易语言名称或一组使用','或'.'号连接起来的易语言名称，且其中不能包含转义符；
        B、','和'.'将被认为是文本的一部份，如： “名称 = sys.object”，“列表 = p1,p2,p3”等。
           ','或'.'号后必须跟一个有效名称，如"列表 = p1,,p3”将是错误的，因为第二个逗号后没有跟一个有效名称。
           如果不能满足此要求，请使用第4条中的文本值格式。
    6、逻辑型属性值可以为“真”或“假”，如果某个属性在属性表中只有名称而没有提供值，则认为是逻辑型属性，其值为真。
*/
/*
    m_dwNameHashValue为m_szName的hash值
    m_dwReserved1必须保持为0
*/
#define _EF_DEFINE_GENERAL_INFO     \
            EFDWord m_dwState;      \
            EFChar *m_szName, *m_szHelpString, *m_szExtendedPropertyList;  \
            EFDWord m_dwNameHashValue;  \
            EFDWord m_dwReserved1;

#define _EF_DEFINE_BASE_TYPE_INFO    \
            _EF_DEFINE_GENERAL_INFO  \
            EFLibInfo* m_pLibInfo;   \
            EFTypeType m_enType;     \
            EFDouble m_dbVersion;    \
            EFInt m_nCategoryIndex;

//////////////////////////////////////////////////////////////////////////
// 基本的m_dwState值

#define EF_S_HIDDEN     (1 << 31)  //   说明具有此标记的类型或成员由于某种原因希望被隐藏起来而不让用户知晓，因此
                                   // 探测器类程序应该将其跳过而不列出。
                                   //   此标记不影响正常使用，仅用作辅助说明。
#define EF_IS_STATE_VISIBLE(state)      (((state) & EF_S_HIDDEN) == 0)

/*
成员的输出属性：
    私有：只能在本类中使用。
    扩展：不可在外部被使用，但可以在本类或继承类中使用。
    公开：可在外部被使用
    如果EF_S_PROTECTED和EF_S_PUBLIC都没有被设置，则为私有。
类型的输出属性：
    私有：只能在本类库中使用。
    公开：可以在本类库外使用。
*/
#define EF_S_PROTECTED  (1 << 30)  // 指定当前成员输出属性为扩展
#define EF_S_PUBLIC     (1 << 29)  // 指定当前成员输出属性为公开

#define EF_IS_STATE_PRIVATE(state)      (((state) & (EF_S_PUBLIC|EF_S_PROTECTED)) == 0)

//////////////////////////////////////////////////////////////////////////

// 三种扩展类型：类，接口，枚举
typedef enum
{
    EF_TT_NULL        = 0,
    EF_TT_CLASS       = 1,
    EF_TT_INTERFACE   = 2,
    EF_TT_ENUM        = 3
}
EFTypeType;

typedef struct _tagEFTypeInfo
{
    _EF_DEFINE_BASE_TYPE_INFO
}
EFTypeInfo;

typedef struct _tagEFGeneralInfo
{
    _EF_DEFINE_GENERAL_INFO
}
EFGeneralInfo;


// 用作快速数据类型操作。
typedef struct _tagEFDataTypeInfo
{
    EF_DatatypeType m_enDataType;  // 记录数据类型类别
	EFChar* m_szDataType;          // 记录数据类型名称
    EFDWord m_dwDataTypeHash;      // 记录数据类型名称的hash值。
    EFTypeInfo* m_pTypeInfo;       // 如果是非基本数据类型，则在此记录其具体类型信息(EFClassInfo/EFInterfaceInfo/EFEnumInfo)，否则为NULL。
}
EFDataTypeInfo;

//////////////////////////////////////////////////////////////////////////

typedef struct _tagEFLocalVarDebugInfo
{
	EFInt m_nLineIndex;    // 该变量在源代码文件中定义时的行索引位置（从0开始）。
    EFChar* m_szDataType;  // 数据类型
    EFChar* m_szName;      // 变量名称
    EFInt m_nDataOffset;   //   变量数据在堆栈中的偏移。如果大于等于零，表明为静态局部变量，
                          // 此时为该变量的替代类变量在EFClassInfo的m_pFieldTable表中的索引。
}
EFLocalVarDebugInfo;

typedef struct _tagEFCodeBlockDebugInfo
{
    EFInt m_nCodeOffset;  // 代码块首地址相对于m_func的偏移量。
    EFInt m_nCodeSize;    // 代码块的尺寸。

    EFInt m_nLocalVarCount;  // 本代码块中局部变量的数目。
    EFLocalVarDebugInfo* m_pLocalVarInf;  // 按照在程序中的定义顺序排序

    // 本代码块中内嵌方法的名称表，中间用'\0'分隔，最后用'\0'结束。如果没有内嵌方法，本成员为NULL。
	EFChar* m_szzInnerMethodsName;
}
EFCodeBlockDebugInfo;

typedef struct _tagEFStatementLineDebugInfo
{
	EFInt m_nLineIndex;        // 语句行在源代码文件中的行索引位置（从0开始）。
    EFInt m_nCodeOffset;	   // 语句行代码数据相对于m_func的偏移量。
	EFInt m_nCallFuncCount;    // 本行所调用的其他方法数目。
	EFInt* m_pnCallCodeOffset; // 记录每个调用到某方法的call指令相对于m_func的偏移量。
    // 记录除了顺序的下一行外，本语句行执行后还可能跳到的代码地址相对于m_func的偏移量，为-1时无效。
    EFInt m_nGotoCodeOffset1, m_nGotoCodeOffset2;
}
EFStatementLineDebugInfo;

// 记录方法调试信息
typedef struct _tagDebugMethodInf
{
    EFChar* m_szSourceFileName;  // 本方法所处源代码文件名称
    EFInt m_nLineIndex;  // 本方法定义头在源代码文件中的行索引位置（从0开始）。
    EFInt m_nCodeSize;   // m_func所指向代码的尺寸。

    EFInt m_nStatementLineCount;
    EFStatementLineDebugInfo* m_pStatementLineInf;  // 按照EFStatementLineDebugInfo.m_nLineIndex排序

    EFInt m_nCodeBlockInfCount;
    EFCodeBlockDebugInfo* m_pCodeBlockInf;  // 按照EFCodeBlockDebugInfo.m_nCodeOffset排序
}
EFMethodDebugInfo;

//////////////////////////////////////////////////////////////////////////

typedef union _tagEFImmValue
{
    EFInt m_int;
    EFDouble m_double;
    EFBool m_bool;
    EFText m_text;
    EFBinary m_bin;
    EFArray m_array;  // 不能为对象数组和接口数组
}
EFImmValue;

typedef union _tagEFValue
{
    EFInt m_int;
    EFDouble m_double;
    EFBool m_bool;
    EFText m_text;
    EFBinary m_bin;
    EFObject* m_pObject;
    EFInterface* m_pInterface;
    EFArray m_array;
}
EFValue;

//////////////////////////////////////////////////////////////////////////

typedef struct _tagEFArgInfo
{
    #define EF_S_ARG_NULLABLE     (1 << 0)       //   如果本参数的数据类型为类或接口，设置本标志位表示可以接收
                                                 // 空的接口或类对象，否则空对象将不能被传递到方法实现代码。
    #define EF_S_ARG_HAS_DEFAULT_VALUE (1 << 1)  // 标记本参数是否具有默认值
    /*
        标记从本参数处到最后一个参数止是否为可扩展参数组，整个参数表中最多只能有一个参数具有此标志。
        举例，如果有方法： method1 (整数 arg1, 整数 arg2<可扩展>, 文本 arg3)。其中，arg2具有EF_S_ARG_EXTENDABLE
      标志，则调用方可以以类似 method1 (1,  2, "abc",  3, "bcd") 的方式对此方法进行参数扩展调用。
        注意，参数只能成组扩展，譬如上面的例子就不能写成 method1 (1,  2, "abc",  3) 。
        在用C++代码实现此方法时，该方法的实现函数应该类似如下书写：
          void method1 (EFObject* pThis, EFInt arg1, EFInt arg2, EFText arg3, EFInt nExtendedArgGroupCount, ...)
        其中，nExtendedArgGroupCount记录具体扩展提供的参数组数目，譬如在上面的例子中，nExtendedArgGroupCount为1。
        如果具有扩展参数的方法同时具有多返回值，则多返回值接收表应放在nExtendedArgGroupCount的前面，C++实现函数类似如下书写：
          void method1 (EFObject* pThis, EFInt arg1, EFInt arg2, EFText arg3, void* pReturnData, EFInt nExtendedArgGroupCount, ...)
    */
    #define EF_S_ARG_EXTENDABLE      (1 << 2)

    _EF_DEFINE_GENERAL_INFO

    EFChar* m_szDataType;
    EFDWord m_dwDataTypeHashValue;  // m_szDataType的hash值。

    //   如果EF_S_ARG_HAS_DEFAULT_VALUE被置位，则本成员用作记录对应的参数默认值。
    // 注意如果参数数据类型为类或接口，则本成员没有作用，其默认值固定为“空”。
    EFImmValue m_vDefault;
}
EFArgInfo;

// 所有事件处理方法的原型
// 返回真表示需要继续传递此事件, 返回假表示中止传递此事件。
// pEventObject指向一个基类为“系统.事件”的对象。
typedef EFBool (*EF_PFN_EVENT_FUNC) (EFObject* pThis, EFObject* pEventObject);  // 非静态的事件处理方法
typedef EFBool (*EF_PFN_STATIC_EVENT_FUNC) (EFObject* pEventObject);  // 静态的事件处理方法

/*   定义多返回值的方法：此时m_szDataType可以按照类似以下方法书写： "int, TextFile"，
   同时方法在编译时将在最后一个参数后面自动加入一个指向结构的指针（不需要手工定义），
   该结构中的成员为所有返回值的顺序排列，返回前把所需要返回的数据填入到该结构中即可。 
     譬如方法：“整数,文本 method1 ()”的C++实现函数应类似如下书写：
        void method1 (EFObject* pThis, void* pReturnData)
*/
typedef struct _tagEFMethodInfo
{
    #define EF_S_M_PROPERTY                (1 << 0)  //   指示本方法是否为属性读写方法。如果定义了此标志，本方法的原型必须类似
                                                     //“返回值数据类型 属性名 ()”-属性读 或“属性名 (参数数据类型 参数名)”-属性写。
    #define EF_S_M_STATIC                  (1 << 1)  // 是否为静态方法。
    #define EF_S_M_EVENT_HANDLER           (1 << 2)  // 是否为事件处理方法，如果具有此标志，则方法的原型必须为EF_PFN_EVENT_FUNC。
    #define EF_S_M_INIT                    (1 << 3)  // 是否为构造方法。如果此标志被设置，上面标志应该都不能被设置。
                                                     // 注意：类的构造方法中必须要调用其基础类的构造方法，以保证基础类对象能够被
                                                     // 构造，有两种调用需求情况：
                                                     // 1、调用基础类的默认构造方法，可以使用公用函数MCallBaseClassDefaultConstructor。
                                                     // 2、调用基础类的指定构造方法，需要自行编码直接调用。
    #define EF_S_M_ENUMERATER              (1 << 4)  // 指示本方法为遍历器类中的遍历方法，具体请参见技术白皮书。
    #define EF_S_M_CONVERTER               (1 << 5)  //   指示本方法是否为数据类型转换器方法，如果定义了此标志，本方法的原型必须
                                                     // 类似“转换到数据类型 方法名 ()” 或“ 初始化 (转换入数据类型 参数名)”。
                                                     // 注意：1、转入方法必须同时是类的初始化方法。
                                                     //       2、只支持对基本数据类型或数组数据类型的转换。
                                                     //       3、转换方法必须公开。
    _EF_DEFINE_GENERAL_INFO

    // 方法友好名，用于实现语意化编程。其中以<1>表示第一个参数，<2>表示第二个参数，如此类推。
    // 允许使用除半角字符“'<”之外的所有字符。
	EFChar* m_szFriendlyName; 
    EFDWord m_dwFriendlyNameHashValue;  // m_szFriendlyName的hash值。

    EFChar* m_szReturnDataType;  // 注意此处可能指向组合数据类型，即类似: "int, TextFile"
    EFDWord m_dwReturnDataTypeHashValue;  // m_szReturnDataType的hash值。

    EFInt m_nArgCount;
    EFArgInfo* m_pArgTable;

    // 如果为接口方法，必定为NULL，否则不能为NULL。
    EF_PFUNC m_func;

    // 如果不为NULL，记录关于本方法的调试信息。
    // 如果在接口中，本信息将被转移到EFInterfaceItem信息中记录，此处必定为NULL。
    EFMethodDebugInfo* m_pDebugInf;
}
EFMethodInfo;


#define EF_IS_PRIVATE_METHOD(pMethodInfo)  \
    EF_IS_STATE_PRIVATE ((pMethodInfo)->m_dwState)

// 判断指定类中定义的方法是否会被记录到虚拟调用表中（注意，本宏不适合于接口中定义的方法，接口中的方法必定是虚拟的）。
#define EF_IS_VIRTUAL_CLASS_METHOD(pMethodInfo)  \
    (((pMethodInfo)->m_dwState & EF_S_M_STATIC) == 0 &&  \
         EF_IS_PRIVATE_METHOD (pMethodInfo) == false)


typedef struct _tagEFFieldInfo
{
    #define EF_S_F_STATIC            (1 << 0)  // 是否为静态成员
    #define EF_S_F_CONST             (1 << 1)  // 是否为常量，注意常量的数据类型不可能为类或接口。
    #define EF_S_F_HAS_INITIAL_VALUE (1 << 2)  //   本成员是否有初始值, 常量成员本标志默认置位。如果本标志或常量标志置位且数据类
                                               // 型的EF_DatatypeType为EF_DT_CLASS_INTERFACE_ENUM或EF_DT_CLASS_INTERFACE_ENUM_ARRAY，
                                               // 则说明该数据类型为枚举或枚举数组，其初始值被记录在m_vInit.m_int或m_vInit.m_array中。
    _EF_DEFINE_GENERAL_INFO

    EFChar* m_szDataType;
    EFDWord m_dwDataTypeHashValue;  // m_szDataType的hash值。

    // 如果EF_S_F_STATIC被置位，则本成员用作记录运行时的静态值。
    EFValue m_vStatic;
    // 如果EF_S_F_CONST或EF_S_F_HAS_INITIAL_VALUE被置位，则本成员用作记录对应的初始值。
    EFImmValue m_vInit;
}
EFFieldInfo;

struct EFInterfaceInfo
{
    #define EF_S_I_FINAL     (1 << 0)    // 不能从本接口再派生其它接口。

    _EF_DEFINE_BASE_TYPE_INFO

    EFChar* m_szBaseInterfaceName;  // 可以为NULL或空文本，注意如果有必须是全名。
    EFDWord m_dwBaseInterfaceNameHashValue;  // m_szBaseInterfaceName的hash值。

    // 注意:
    // 1、这个表一旦定义并公开后则不可进行任何更改, 想要扩充只能通过继承新接口来实现.
    // 2、方法中的EF_S_M_xxx状态值不被支持。
    EFInt m_nMethodCount;
    EFMethodInfo* m_pMethodTable;
};
typedef struct EFInterfaceInfo EFInterfaceInfo;

typedef struct _tagEFInterfaceItem
{
    EFChar* m_szInterfaceName;  // 所实现的接口名称，注意应该填写全名。
    EFDWord m_dwNameHashValue;  // 该名称的hash值。

    EFDWord m_dwReserved1;  // 必须保持为0

    EFInt m_nFuncCount;
    EF_PFUNC* m_pFunc;  // 注意: 这个表中项目的顺序和数目必须与接口定义完全一致。如有基础接口, 基础接口的方法表应记录在前。
    EFMethodDebugInfo** m_ppDebugInf;  //  如果不为NULL，则顺序记录上面所有方法的调试信息，此时m_ppDebugInf中的指针数目等于m_nFuncCount。
}
EFInterfaceItem;


typedef void (*EF_PFN_CLASS_LOADED)   (EFClassInfo* pClassInfo);  // pClassInfo为被载入的类信息
typedef void (*EF_PFN_OBJECT_CREATED) (EFObject* pObject);        // pObject为当前对象
typedef void (*EF_PFN_OBJECT_CLEAN)   (EFObject* pObject);        // pObject为当前对象

//   如果调用时ppRefData提供为NULL，则返回对象数据中所有在CLASSINFO的m_pFieldTable表中没有相应定义项的参考类数据项数目。
//   如果调用时ppRefData不为NULL，则将所有以上数据值顺序填入该内存地址中。
//   具体见EFClassInfo中对于m_pfnGCRefDataGetter的说明。
typedef int (*EF_PFN_GC_REF_DATA_GETTER)  (EFObject* pObject, void** ppRefData);

typedef enum
{
    EF_COMPILER_UNKNOWN, // 未知编译器
    EF_COMPILER_EF,      // 易语言编译器
    EF_COMPILER_VC,      // VC++ 
    EF_COMPILER_GCC      // GCC++
}
EF_CompilerType;

//!! 如果在下面各种操作系统下使用的不是所定义的编译器，则应作相应修改。
#ifdef _LINUX
    #define _EF_CURRENT_COMPILER  EF_COMPILER_GCC
#elif WIN32
    #define _EF_CURRENT_COMPILER  EF_COMPILER_VC
#else
    #error EF_OS type required.
    #define _EF_CURRENT_COMPILER  EF_COMPILER_UNKNOWN
#endif

struct EFClassInfo
{
    #define EF_S_CLS_NO_DEFAULT_INIT  (1 << 0)         //   对于所有类，编译器都默认其具有一个公开的无参数的空白“初始化”方法。
                                                       // 如果不希望具有此默认设置，加上此状态值即可。
                                                       //   注意，如果定义了此标志却没有提供任何一个“初始化”方法，则恢复
                                                       // 默认设置（等同于未定义此标志）。
    #define EF_S_CLS_FINAL                   (1 << 1)  // 是否为最终类，不能从本类再派生其它类。
    #define EF_S_CLS_ALLOW_ACCESS_PARENT     (1 << 2)  //   如果本类为嵌入类，标记本类能否允许访问其所在类型的非私有成员和方法。
                                                       // 如果允许，则本类必有名为EF_NAME_PARENT_OBJECT_FIELD_E的数据成员，
                                                       // 用作指向其所在类或接口的对象实例。注意：在创建具有此标记的嵌入类对象
                                                       // 实例后，必须立即首先填写此成员(包括在调用该类任一初始化方法前)，否则
                                                       // 后续调用将可能会出错。

    _EF_DEFINE_BASE_TYPE_INFO  // 注意，类名为全指定名。

    EF_CompilerType m_enCompilerType;  // 编译本类方法代码的编译器类型。

    EFChar* m_szBaseClassName;
    EFDWord m_dwBaseClassNameHash;

    EFInt m_nInterfaceCount;  // 本类所实现接口的数目。
    EFInterfaceItem* m_pInterfaceTable;    // 指向所实现接口的数组。

    EF_PFN_CLASS_LOADED m_fnClassInit;     // 当本类被载入系统的时候调用，可以用作初始化类中所有的静态数据成员，可以为NULL。
    EF_PFN_OBJECT_CREATED m_pfnObjectInit; //   当基于本类的对象被创建时调用，可以为NULL。注意m_pfnObjectInit不等同于类
                                           // 的构造函数，类的构造函数是可以被继承类的构造函数跳过去的（不调用其基础类的
                                           // 构造函数），但m_pfnObjectInit在任何情况下都将被调用。本成员的主要用途是为
                                           // C++写的类提供创建相关本类对象的额外数据的机会。它在类的构造函数调用之前调用。
    EF_PFN_OBJECT_CLEAN m_pfnObjectClean;  // 当基于本类的对象将被释放的时候调用，用作进行额外的清理工作，可以为NULL。

    //   如果本类数据区中保存有一些非静态非常量参考类数据项（文本、字节集、类对象、接口对象、数组等），且这些数据未
    // 在后面的m_pFieldTable表中具有相应定义项，则必须实现此方法。实现此方法后当GC访问到此类的对象时，会通过额外
    // 调用此方法来动态获取这些参考类数据并作相应处理以保证不会出现内存垃圾。
    //   作用：通过此特性可以实现复杂、动态的对象数据存储，而不必担心由此让GC访问不到而形成内存垃圾。
    // 注意：
    //   1、一般情况下无需使用本特性，也无需掌握了解；
    //   2、静态和常量参考类数据不能通过本方式保存，必须在m_pFieldTable表中具有其相应定义项。
    //   3、如果m_pFieldTable表中没有相应数据的定义项，该数据将无法通过“对象.成员”方式来访问，因此有必要提供相应
    // 的成员方法来让用户访问到这些额外数据。
    EF_PFN_GC_REF_DATA_GETTER m_pfnGCRefDataGetter;

    EFInt m_nMethodCount;
    EFMethodInfo* m_pMethodTable;  // 注意: 这个表中项目的顺序一旦定义并公开则在以后不可删除或更改索引位置.

    EFInt m_nFieldCount;
    EFFieldInfo* m_pFieldTable;  // 注意: 这个表中项目的顺序一旦定义并公开则在以后不可删除或更改索引位置.

    EFInt m_nExportEventCount;
    EFChar** m_szaryExportEventClassName;
        //   记录可能抛出事件对象的类名称,可以是全名或简名，如："Window.ClickEvent" / "ClickEvent" 等格式。
        // 简名表示使用当前类库中的类。
        //   这个表中项目的顺序在升级类的时候可以变动索引位置或者删除.
        //   如果事件名称以'~'开头，表示屏蔽基础类中对应名称的输出事件，如：
        // 假设有个button类定义了一个"button_clicked"类事件输出，那么如果定义了一个"newbutton"类从其继承，
        // 然后在其事件输出表中加入"~button_clicked"记录项，则表示将基础类中的"button_clicked"事件
        // 屏蔽掉不再输出。
};
typedef struct EFClassInfo EFClassInfo;

/////////////////////////////////////

typedef struct _tagEFEnumItem
{
    _EF_DEFINE_GENERAL_INFO;
    EFInt m_int;  // 枚举值 
}
EFEnumItem;

typedef struct _tagEFEnumInfo
{
    _EF_DEFINE_BASE_TYPE_INFO

    EFInt m_nItemsCount;
    EFEnumItem* m_pItems;
}
EFEnumInfo;

//////////////////////////////////////////////////////////////////////////

// 适用语言定义:
typedef enum
{
    EF_LANGUAGE_CHINESE    = 0,  // 中文（中国大陆）
    EF_LANGUAGE_BIG5       = 1,  // 中文（中国台湾）
    EF_LANGUAGE_ENGLISH    = 2,  // 英语
	EF_LANGUAGE_KOREAN     = 3,  // 韩文
	EF_LANGUAGE_JAPANESE   = 4   // 日文
}
EF_LanguageType;

//////////////////////////////

// 适用操作系统定义:
#define EF_OS_WINDOWS  (1 << 0)
#define EF_OS_LINUX    (1 << 1)
#define EF_OS_UNIX     (1 << 2)
#define EF_OS_ALL      (EF_OS_WINDOWS | EF_OS_LINUX | EF_OS_UNIX)

//////////////////////////////

#define EF_OK		0
#define EF_ERROR	-1

//////////////////////////////////////////////////////////////////////////
// 系统可以通知库的码值（>=0）：
typedef enum
{
    EF_NOTIFY_LIB_LOAD  = 1,
	    // 告知库其已经被装载，在得到此通知后库代码可以做必要的初始化工作。这是库得到的第一个通知。
	    // dwParam1: (EFObject*)指针，为系统运行时环境的Runtime对象。
        // 返回值：忽略。
    EF_NOTIFY_LIB_UNLOAD = 3,
	    // 告知库其即将被卸载，在得到此通知后库代码请做必要的最终清理工作。这是库得到的最后一个通知。
        // 返回值：忽略。
}
EF_NotifyLibCode;

typedef EFInt (*EF_PFN_NOTIFY_LIB) (EFLibInfo* pLibInfo, EF_NotifyLibCode enMsg, EFDWord dwParam1, EFDWord dwParam2);  // 此函数用作系统通知库有关事件。

typedef EFInt (*EF_PFN_GET_TYPE_INFOS) (EFLibInfo* pLibInfo, EFTypeInfo** ppTypeInfoBuf, EFInt nBufItemCount);  // ppTypeInfoBuf如果为NULL则返回库中的类型数目。
typedef EFTypeInfo* (*EF_PFN_FIND_TYPE_INFO) (EFLibInfo* pLibInfo, EFChar* szTypeInfoName);

#define EF_LIB_FORMAT_VER_1  1
#define EF_CURRENT_LIB_FORMAT_VER  EF_LIB_FORMAT_VER_1

struct EFLibInfo
{
    #define EF_S_LIB_IS_EFLIB  (1 << 0)  // 标志是否为EF本身编写的类库

    _EF_DEFINE_GENERAL_INFO;

    EFInt m_nFormatVersion;  // 本结构的格式版本号,以备以后更改本数据结构的格式用.

	EF_LanguageType m_nLanguage;  // 库中所有文本所使用的语言。
    EFDWord m_dwSupportedOS;      // 记录本库具有哪些操作系统支持版本。
    EFDouble m_dbVersion;         // 库的版本号
	EFInt m_nBuildNumber;
		// 库构建版本号，无需对此版本号作任何处理。
		// 本版本号仅用作区分相同正式版本号的系统软件（譬如仅仅修改了几个 BUG，不值得升级正式版本的系统软件）。
        // 任何公布过给用户使用的版本其构建版本号都应该不一样。 赋值时应该顺序递增。

	EFChar* m_szzTypeInfoCategory;  // 数据类型的类别说明表，中间用'\0'分隔，最后用'\0'结束，格式如："算术\0逻辑\0\0"
	EFChar* m_szzDependFiles;       // 本库正常运行所需要依赖的其他辅助文件名，中间用'\0'分隔，最后用'\0'结束，格式如："c:\\abc.txt\0d:\\bcd.txt\0\0"

    EFChar* m_szStarterClassName;   //   启动类名称，当从命令行启动类库时候将自动载入库中的具有本成员名称的类，
                                    // 然后执行其中名称为EF_NAME_MAIN_METHOD的方法。

    EF_PFN_GET_TYPE_INFOS m_pfnGetTypeInfos;
    EF_PFN_FIND_TYPE_INFO m_pfnFindTypeInfo;
    EF_PFN_NOTIFY_LIB m_pfnNotifyLib;  // 接收运行时环境通知的函数,不能为NULL
    
    EFChar* m_szAuthor;  // 库的作者
	EFChar* m_szOther;   // 其它信息，比如版权信息、网址等。

    // 如果标志EF_S_LIB_IS_EFLIB被置位，则下面两成员必须填写，否则不需要填写。
    EFInt m_nTypesCountInELib;
    EFTypeInfo** m_ppTypesInfoInELib;

    // 用作保留关于此库的自定义信息
    EFInt m_nCustomData;
};
typedef struct EFLibInfo EFLibInfo;


//////////////////////////////////////////////////////////////////////////

class _EF_TypeInfoRecorder
{
public:
    _EF_TypeInfoRecorder (EFTypeInfo* pBaseTypeInfo);

    static void CleanUp ();

    static EF_Text2PtrHashmap* sm_pBaseTypeInfoTable;
};

class _EF_TypeInfoCleaner
{
public:
    _EF_TypeInfoCleaner ()  { }
    ~_EF_TypeInfoCleaner ()
    {
        _EF_TypeInfoRecorder::CleanUp ();
    }
};

//////////////////////////////////////////////////////////////////////////


#define EF_METHOD_TABLE(class_en_name)           _ef_method_table_of_##class_en_name
#define EF_METHOD_COUNT(class_en_name)           (sizeof (_ef_method_table_of_##class_en_name) / sizeof (_ef_method_table_of_##class_en_name [0]))

#define EF_FIELD_TABLE(class_en_name)            _ef_field_table_of_##class_en_name
#define EF_FIELD_COUNT(class_en_name)            (sizeof (_ef_field_table_of_##class_en_name) / sizeof (_ef_field_table_of_##class_en_name [0]))

#define EF_EXPORT_EVENT_TABLE(class_en_name)     _ef_export_event_table_of_##class_en_name
#define EF_EXPORT_EVENT_COUNT(class_en_name)     (sizeof (_ef_export_event_table_of_##class_en_name) / sizeof (_ef_export_event_table_of_##class_en_name [0]))

#define EF_FRIENDLY_NAME_TABLE(class_en_name)        _ef_friendly_name_table_of_##class_en_name
#define EF_FRIENDLY_NAME_COUNT(class_en_name)        (sizeof (_ef_friendly_name_table_of_##class_en_name) / sizeof (_ef_friendly_name_table_of_##class_en_name [0]))

#define EF_INTERFACE_IMPLEMENT_TABLE(class_en_name)  _ef_interface_implement_table_of_##class_en_name
#define EF_INTERFACE_IMPLEMENT_COUNT(class_en_name)  (sizeof (_ef_interface_implement_table_of_##class_en_name) / sizeof (_ef_interface_implement_table_of_##class_en_name [0]))

#define EF_ENUM_ITEM_TABLE(enum_en_name)    _ef_enum_item_table_of_##enum_en_name
#define EF_ENUM_ITEM_COUNT(enum_en_name)    (sizeof (_ef_enum_item_table_of_##enum_en_name) / sizeof (_ef_enum_item_table_of_##enum_en_name [0]))

///////////////////////////////////

#define EF_BEGIN_DEFINE_ARG_TABLE(class_en_name, method_en_name)  EF_STATIC EFArgInfo _ef_arg_table_of_##class_en_name##_##method_en_name [] = {

#define EF_ARG_HASPROP(name, help_string, state, properties_list, data_type) \
            {  state, name, help_string, properties_list, EF_GetStrHash (name), 0, data_type, EF_GetStrHash (data_type), { 0 } }

#define EF_ARG(name, help_string, state, data_type) \
            EF_ARG_HASPROP(name, help_string, state, myT(""), data_type)

#define EF_ARG_HAS_DEFAULT(name, help_string, state, data_type, default_value) \
            {  state|EF_S_ARG_HAS_DEFAULT_VALUE, name, help_string, myT(""), EF_GetStrHash (name), 0, data_type, EF_GetStrHash (data_type), { (int)(default_value) } }

#define EF_END_DEFINE_ARG_TABLE  };

///////////////////////////////////

#define EF_BEGIN_DEFINE_FIELD_TABLE(class_en_name)  EF_STATIC EFFieldInfo _ef_field_table_of_##class_en_name [] = {

#define EF_FIELD_HASPROP(name, help_string, state, properties_list, data_type) \
            {  state, name, help_string, properties_list, EF_GetStrHash (name), 0, data_type, EF_GetStrHash (data_type), { 0 }, { 0 }  }

#define EF_FIELD(name, help_string, state, data_type) \
        EF_FIELD_HASPROP (name, help_string, state, myT (""), data_type)

#define EF_END_DEFINE_FIELD_TABLE  };

///////////////////////////////////

#define EF_BEGIN_DEFINE_INTERFACE_IMPLEMENT_TABLE(class_en_name)  EF_STATIC EFInterfaceItem _ef_interface_implement_table_of_##class_en_name [] = {

#define EF_IMPLEMENT_INTERFACE(interface_name, func_count, func_tab) \
            {  interface_name, EF_GetStrHash (interface_name), 0, func_count, func_tab, NULL  }

#define EF_END_DEFINE_INTERFACE_IMPLEMENT_TABLE  };

///////////////////////////////////

#define EF_BEGIN_DEFINE_METHOD_TABLE(class_en_name)  EF_STATIC EFMethodInfo _ef_method_table_of_##class_en_name [] = {

#define EF_END_DEFINE_METHOD_TABLE  };

#define EF_METHOD_HASPROP(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  state, method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name) / sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name [0]), _ef_arg_table_of_##class_en_name##_##method_en_name, (EF_PFUNC)class_en_name##_##method_en_name, NULL  }

#define EF_METHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  state, method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), 0, (EFArgInfo*)0, (EF_PFUNC)class_en_name##_##method_en_name, NULL  }

#define EF_METHOD(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_METHOD_HASPROP(class_en_name, method_en_name, method_name,  method_friend_name, help_string, state, myT(""), return_datatype)

#define EF_METHOD_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_METHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, myT(""), return_datatype)

#define EF_PMETHOD_HASPROP(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  (EF_S_PUBLIC | state), method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name) / sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name [0]), _ef_arg_table_of_##class_en_name##_##method_en_name, (EF_PFUNC)class_en_name##_##method_en_name, NULL  }

#define EF_PMETHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  (EF_S_PUBLIC | state), method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), 0, (EFArgInfo*)0, (EF_PFUNC)class_en_name##_##method_en_name, NULL  }

#define EF_PMETHOD(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_PMETHOD_HASPROP(class_en_name, method_en_name, method_name,  method_friend_name, help_string, state, myT(""), return_datatype)

#define EF_PMETHOD_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_PMETHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, myT(""), return_datatype)

///////////////////////////////////

#define EF_INTERFACE_METHOD_HASPROP(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  state, method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name) / sizeof (_ef_arg_table_of_##class_en_name##_##method_en_name [0]), _ef_arg_table_of_##class_en_name##_##method_en_name, (EF_PFUNC)NULL, NULL  }

#define EF_INTERFACE_METHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, properties_list, return_datatype) \
            {  state, method_name, help_string, properties_list, EF_GetStrHash (method_name), 0, method_friend_name, EF_GetFriendlyNameHash (method_friend_name), return_datatype, EF_GetStrHash (return_datatype), 0, (EFArgInfo*)0, (EF_PFUNC)NULL, NULL  }

#define EF_INTERFACE_METHOD(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_INTERFACE_METHOD_HASPROP(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, myT(""), return_datatype)

#define EF_INTERFACE_METHOD_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, return_datatype) \
            EF_INTERFACE_METHOD_HASPROP_NOARG(class_en_name, method_en_name, method_name, method_friend_name, help_string, state, myT(""), return_datatype)

///////////////////////////////////

#define EF_BEGIN_DEFINE_EXPORT_EVENT_TABLE(class_en_name)  EF_STATIC EFChar* _ef_export_event_table_of_##class_en_name [] = {

#define EF_EXPORT_EVENT(event_class_name)  event_class_name

#define EF_END_DEFINE_EXPORT_EVENT_TABLE  };

///////////////////////////////////

#define EF_BEGIN_DEFINE_ENUM_ITEM_TABLE(enum_en_name)  EF_STATIC EFEnumItem _ef_enum_item_table_of_##enum_en_name [] = {

#define EF_ENUM_ITEM_HASPROP(enum_item_name, help_string, state, properties_list, value)  \
    {  state, enum_item_name, help_string, properties_list, EF_GetStrHash (enum_item_name), 0, value }

#define EF_ENUM_ITEM(enum_item_name, value, help_string)  \
    EF_ENUM_ITEM_HASPROP (enum_item_name, help_string, EF_S_PUBLIC, myT (""), value)

#define EF_ENUM_ITEM_WITH_STATE(enum_item_name, value, help_string, state)  \
    EF_ENUM_ITEM_HASPROP (enum_item_name, help_string, state, myT (""), value)

#define EF_END_DEFINE_ENUM_ITEM_TABLE  };

///////////////////////////////////

// EF_DEFINE_CLASS_INFO and EF_DEFINE_ENUM_INFO must be used in .cpp (not .h) file.

#define EF_DEFINE_CLASS_INFO_HAS_REF_DATA_GETTER(version, class_en_name, class_name, help_string, base_class_name, interface_implement_count, interface_implement_table, state, category_index, properties_list, class_init_func, object_init_func, object_clean_func, ref_data_getter_func, method_count, method_table, element_count, element_table, export_event_count, export_event_table)  \
        EFClassInfo _ef_class_info_of_##class_en_name = {  \
                state,                          \
                class_name,                     \
                help_string,                    \
                properties_list,                \
                EF_GetStrHash (class_name),     \
                0,                              \
                &ef_LibInfo,                    \
                EF_TT_CLASS,                    \
                version,                        \
                category_index,                 \
                _EF_CURRENT_COMPILER,           \
                base_class_name,                \
                EF_GetStrHash (base_class_name),\
				interface_implement_count,      \
				interface_implement_table,      \
                class_init_func,                \
                object_init_func,               \
                object_clean_func,              \
                ref_data_getter_func,           \
                method_count,                   \
                method_table,                   \
                element_count,                  \
                element_table,                  \
                export_event_count,             \
                export_event_table              \
        };                                      \
        EF_STATIC _EF_TypeInfoRecorder _ef_class_info_recorder_of_##class_en_name ((EFTypeInfo*)&_ef_class_info_of_##class_en_name);

// 定义类
#define EF_DEFINE_CLASS_INFO(version, class_en_name, class_name, help_string, base_class_name, interface_implement_count, interface_implement_table, state, category_index, properties_list, class_init_func, object_init_func, object_clean_func, method_count, method_table, element_count, element_table, export_event_count, export_event_table)  \
    EF_DEFINE_CLASS_INFO_HAS_REF_DATA_GETTER(version, class_en_name, class_name, help_string, base_class_name, interface_implement_count, interface_implement_table, state, category_index, properties_list, class_init_func, object_init_func, object_clean_func, NULL, method_count, method_table, element_count, element_table, export_event_count, export_event_table)

// 定义简单类
#define EF_DEFINE_LITE_CLASS_INFO(class_en_name, class_name, help_string, base_classname)	\
	EF_DEFINE_CLASS_INFO(1, class_en_name, class_name, help_string, base_classname, 0, NULL, EF_S_PUBLIC, -1, myT(""), NULL, NULL, NULL, 0, NULL, 0, NULL, 0, NULL)	

// 定义只有常量的类
#define EF_DEFINE_CONST_CLASS_INFO(version, class_en_name, class_name, help_string,state,properties_list, category_index)  \
    EF_DEFINE_CLASS_INFO(version, class_en_name, class_name, help_string, CLS_sys_Object, 0, NULL, state, category_index, properties_list, NULL, NULL, NULL, 0, NULL, EF_FIELD_COUNT(class_en_name), EF_FIELD_TABLE(class_en_name), 0, NULL)

//////////////////////////////////////////////////////////////////////////

// 定义常量类中的整数常量成员

#define EF_BEGIN_DEFINE_CONST_FIELD_TABLE(class_en_name)  EF_STATIC EFFieldInfo _ef_field_table_of_##class_en_name [] = {

#define EF_INT_CONST(name, int_value, help_string) \
        {  EF_S_F_CONST | EF_S_PUBLIC | EF_S_F_HAS_INITIAL_VALUE, name, help_string, myT(""), EF_GetStrHash (name), 0, EF_INT, EF_GetStrHash (EF_INT), { 0 }, { int_value }  }

#define EF_END_DEFINE_CONST_FIELD_TABLE	};

//////////////////////////////////////////////////////////////////////////

#define EF_DEFINE_INTERFACE_INFO(version, interface_en_name, interface_name, help_string, base_interface_name, state, category_index, properties_list, method_count, method_table)  \
            EFInterfaceInfo _ef_interface_info_of_##interface_en_name = {  \
                    state,                        \
                    interface_name,               \
                    help_string,                  \
                    properties_list,              \
                    EF_GetStrHash (interface_name), \
                    0,                            \
                    &ef_LibInfo,                  \
                    EF_TT_INTERFACE,              \
                    version,                      \
                    category_index,               \
                    base_interface_name,          \
                    EF_GetStrHash (base_interface_name), \
                    method_count,                 \
                    method_table                  \
            };  \
            EF_STATIC _EF_TypeInfoRecorder _ef_interface_info_recorder_of_##interface_en_name ((EFTypeInfo*)&_ef_interface_info_of_##interface_en_name);

//////////////////////////////////////////////////////////////////////////

#define EF_DEFINE_ENUM_INFO(version, enum_en_name, enum_name, help_string, state, properties_list, category_index)  \
            EFEnumInfo _ef_enum_info_of_##enum_en_name = { \
                    state,                       \
                    enum_name,                   \
                    help_string,                 \
                    properties_list,             \
                    EF_GetStrHash (enum_name),   \
                    0,                           \
                    &ef_LibInfo,                 \
                    EF_TT_ENUM,                  \
                    version,                     \
                    category_index,              \
                    EF_ENUM_ITEM_COUNT(enum_en_name),  \
                    EF_ENUM_ITEM_TABLE(enum_en_name)   \
            };  \
            EF_STATIC _EF_TypeInfoRecorder _ef_enum_info_recorder_of_##enum_en_name ((EFTypeInfo*)&_ef_enum_info_of_##enum_en_name);


//////////////////////////////////////////////////////////////////////////

// 此全局变量由EF_DEFINE_LIB_INFO定义
extern EFLibInfo ef_LibInfo;

// 以下全局变量由EF_DEFINE_EFN_GLOBAL_VARS定义
extern EFObject*     ef_pRuntimeObject;
extern EFClassInfo*  ef_pRuntimeClassInfo;
extern EFClassInfo*  ef_pGCClassInfo;

#define EF_DEFINE_EFN_GLOBAL_VARS  \
            EFObject*     ef_pRuntimeObject = NULL;     \
            EFClassInfo*  ef_pRuntimeClassInfo = NULL;  \
            EFClassInfo*  ef_pGCClassInfo = NULL;

// 4为runtime对象的“取垃圾收集器类”方法的索引，如果该索引有更改，必须同步修改此宏
// 2为runtime对象的“添加常量区”方法的索引，如果该索引有更改，必须同步修改此宏
// 如果没有libinfo，pLibInfo参数可以提供NULL
#define EF_FILL_EFN_GLOBAL_VARS(pRuntimeObject, pLibInfo)  \
            ef_pRuntimeObject = pRuntimeObject;  \
            ASSERT (ef_pRuntimeObject != NULL);  \
            ef_pRuntimeClassInfo = EF_GET_CLASS_INFO (ef_pRuntimeObject);  \
            ef_pGCClassInfo = (*(EFClassInfo* (*) ())ef_pRuntimeClassInfo->m_pMethodTable [4].m_func) ();  \
            ((EFConstDataHeader*)&g_EmptyConst)->m_pLibInfo = pLibInfo;  \
            (*(void (*) (EFConstDataHeader*))ef_pRuntimeClassInfo->m_pMethodTable [2].m_func) ((EFConstDataHeader*)&g_EmptyConst);

#ifdef WIN32
    #define EF_EXPORT __declspec(dllexport)
#else
    #define EF_EXPORT 
#endif

typedef void (*EF_PFN_ON_GET_LIB_INFO) (EFLibInfo* pLibInfo);  // 在 EF_GetLibInfo() 被调用时执行的函数指针，可以在其中修改 pLibInfo 中的数据

// 定义类库信息
// 最后一个参数 pfn_on_get_lib_info 的类型为 EF_PFN_ON_GET_LIB_INFO，可以为NULL
// EF_DEFINE_LIB_INFO must be used in .cpp (not .h) file.
#define EF_DEFINE_LIB_INFO(version, build_number, lib_en_name, lib_name, help_string, state, properties_list, language, os_type, type_info_category, depend_files, author, other_info, pfnOnGetLibInfo)  \
            EF_DEFINE_EFN_GLOBAL_VARS;                                                                                  \
            EF_PFN_ON_GET_LIB_INFO _ef_pfnOnGetLibInfo = pfnOnGetLibInfo;                                               \
            EFInt EF_NotifyLib (EFLibInfo* pLibInfo, EF_NotifyLibCode enMsg, EFDWord dwParam1, EFDWord dwParam2);       \
                                                                                                                        \
            EF_STATIC EFInt _EF_GetTypeInfos (EFLibInfo* pLibInfo, EFTypeInfo** ppTypeInfoBuf, EFInt nBufItemCount) \
            {                                                                                                           \
                if (ppTypeInfoBuf == NULL)                                                                              \
                {                                                                                                       \
                    return (_EF_TypeInfoRecorder::sm_pBaseTypeInfoTable == NULL) ? 0 :                               \
                            _EF_TypeInfoRecorder::sm_pBaseTypeInfoTable->GetCount ();                                \
                }                                                                                                       \
                if (_EF_TypeInfoRecorder::sm_pBaseTypeInfoTable == NULL)                                             \
                    return 0;                                                                                           \
                EFTypeInfo* pTypeInfo;                                                                              \
                EF_Text2PtrHashmap::p2p_iterator iter = (_EF_TypeInfoRecorder::sm_pBaseTypeInfoTable)->begin ();     \
                EFInt i = 0;                                                                                            \
                while (i < nBufItemCount && iter.GetValue ((void**)&pTypeInfo))                                         \
                {                                                                                                       \
                    ppTypeInfoBuf [i++] = pTypeInfo;                                                                    \
                    iter.skip ();                                                                                       \
                }                                                                                                       \
                return i;                                                                                               \
            }                                                                                                           \
            EF_STATIC EFTypeInfo* _EF_FindTypeInfo (EFLibInfo* pLibInfo, EFChar* szTypeInfoName)                    \
            {                                                                                                           \
                if (_EF_TypeInfoRecorder::sm_pBaseTypeInfoTable == NULL)                                             \
                    return (EFTypeInfo*)0;                                                                          \
	            void* value;                                                                                            \
	            if ((_EF_TypeInfoRecorder::sm_pBaseTypeInfoTable)->Get (szTypeInfoName, &value) == EF_FALSE)         \
                    return (EFTypeInfo*)0;                                                                          \
                else                                                                                                    \
                    return (EFTypeInfo*)value;                                                                      \
            }                                                                                                           \
                                                                                                                        \
            EFLibInfo ef_LibInfo = {                                                                                    \
                state,                                                                                                  \
                lib_name,                                                                                               \
                help_string,                                                                                            \
                properties_list,                                                                                        \
                EF_GetStrHash (lib_name),                                                                               \
                0,                                                                                                      \
                EF_CURRENT_LIB_FORMAT_VER,                                                                              \
                language,                                                                                               \
                os_type,                                                                                                \
                version,                                                                                                \
                build_number,                                                                                           \
                type_info_category,                                                                                     \
                depend_files,                                                                                           \
                myT(""),                                                                                                \
                _EF_GetTypeInfos,                                                                                       \
                _EF_FindTypeInfo,                                                                                       \
                EF_NotifyLib,                                                                                           \
                author,                                                                                                 \
				other_info,                                                                                             \
                0,                                                                                                      \
                NULL,                                                                                                   \
                0                                                                                                       \
            };                                                                                                          \
            extern "C" EF_EXPORT EFLibInfo* EF_GetLibInfo ()                                                            \
            {                                                                                                           \
                if (_ef_pfnOnGetLibInfo)                                                                                \
                    (*_ef_pfnOnGetLibInfo) (&ef_LibInfo);                                                               \
                return &ef_LibInfo;                                                                                     \
            }                                                                                                           \
                                                                                                                        \
            EF_Text2PtrHashmap* _EF_TypeInfoRecorder::sm_pBaseTypeInfoTable = (EF_Text2PtrHashmap*)NULL;             \
            EF_STATIC _EF_TypeInfoCleaner _ef_BaseTypeInfoTableCleaner;                                              \
                                                                                                                        \
            _EF_TypeInfoRecorder::_EF_TypeInfoRecorder (EFTypeInfo* pBaseTypeInfo)                            \
            {                                                                                                           \
                if (sm_pBaseTypeInfoTable == NULL)                                                                      \
                    sm_pBaseTypeInfoTable = new EF_Text2PtrHashmap (300, EF_FALSE);                                     \
                sm_pBaseTypeInfoTable->Set (pBaseTypeInfo->m_szName, pBaseTypeInfo);                                    \
            }                                                                                                           \
            void _EF_TypeInfoRecorder::CleanUp ()                                                                    \
            {                                                                                                           \
                if (sm_pBaseTypeInfoTable != (EF_Text2PtrHashmap*)0)                                                    \
                {                                                                                                       \
                    delete sm_pBaseTypeInfoTable;                                                                       \
                    sm_pBaseTypeInfoTable = (EF_Text2PtrHashmap*)0;                                                     \
                }                                                                                                       \
            }


typedef void (*EF_PFN_ON_LOAD_LIB) (EFLibInfo* pLibInfo);    // 在收到 EF_NOTIFY_LIB_LOAD   通知时执行的函数指针
typedef void (*EF_PFN_ON_UNLOAD_LIB) (EFLibInfo* pLibInfo);  // 在收到 EF_NOTIFY_LIB_UNLOAD 通知时执行的函数指针

// 定义默认的通知处理函数
// 两个参数的类型分别是 EF_PFN_ON_LOAD_LIB, EF_PFN_ON_UNLOAD_LIB，都可以为NULL
#define EF_DEFINE_DEFAULT_NOTIFY_LIB_FUNC(pfnOnLoadLib, pfnOnUnloadLib)         \
            EF_PFN_ON_LOAD_LIB   _ef_pfnOnLoadLib   = pfnOnLoadLib;             \
            EF_PFN_ON_UNLOAD_LIB _ef_pfnOnUnloadLib = pfnOnUnloadLib;           \
                                                                                \
            EFInt EF_NotifyLib (EFLibInfo* pLibInfo, EF_NotifyLibCode enMsg, EFDWord dwParam1, EFDWord dwParam2)  \
            {                                                                   \
                switch (enMsg)                                                  \
                {                                                               \
                case EF_NOTIFY_LIB_LOAD:                                        \
                    EF_FILL_EFN_GLOBAL_VARS ((EFObject*)dwParam1, pLibInfo);    \
                    if (_ef_pfnOnLoadLib)                                       \
                        (*_ef_pfnOnLoadLib) (pLibInfo);                         \
                    break;                                                      \
                case EF_NOTIFY_LIB_UNLOAD:                                      \
                    if (_ef_pfnOnUnloadLib)                                     \
                        (*_ef_pfnOnUnloadLib) (pLibInfo);                       \
                    break;                                                      \
                default:                                                        \
                    return EF_ERROR;                                            \
                }                                                               \
                return EF_OK;                                                   \
            }


////////////////////////////////////////////////////////////////////////
// EF_CM_*, 用于调用对象的成员方法（可能已被子类覆盖）
// EF_OM_*, 用于调用对象的成员方法（未被子类覆盖之前）
// EF_CM_*,EF_OM_* 参数中的 vmethod_index 为虚函数索引(>=0)，即，除去静态方法和私有方法之后的方法索引。

#define EF_CM_0(class_index, vmethod_index, ret_datatype, pThis)    \
	(*(ret_datatype (*) (EFObject*)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis)
#define EF_OM_0(class_index, vmethod_index, ret_datatype, pThis)    \
	(*(ret_datatype (*) (EFObject*)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis)

#define EF_CM_1(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value)
#define EF_OM_1(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value)

#define EF_CM_2(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value)
#define EF_OM_2(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value)

#define EF_CM_3(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value)
#define EF_OM_3(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value)

#define EF_CM_4(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value)
#define EF_OM_4(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value)

#define EF_CM_5(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value)
#define EF_OM_5(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value)

#define EF_CM_6(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype,arg6_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value)
#define EF_OM_6(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value)

#define EF_CM_7(class_index, vmethod_index, ret_datatype, pThis, t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6, t7, v7)    \
	(*(ret_datatype (*) (EFObject*, t1, t2, t3, t4, t5, t6,t7)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, v1, v2, v3, v4, v5, v6,v7)
#define EF_OM_7(class_index, vmethod_index, ret_datatype, pThis, t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6, t7, v7)    \
	(*(ret_datatype (*) (EFObject*, t1, t2, t3, t4, t5, t6,t7)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, v1, v2, v3, v4, v5, v6,v7)

#define EF_CM_8(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value)
#define EF_OM_8(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value)

#define EF_CM_9(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value)
#define EF_OM_9(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value)

#define EF_CM_10(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value)
#define EF_OM_10(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value)

#define EF_CM_11(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value, arg11_datatype, arg11_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype,arg11_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value,arg11_value)
#define EF_OM_11(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value, arg11_datatype, arg11_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype,arg11_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value,arg11_value)

#define EF_CM_12(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value, arg11_datatype, arg11_value, arg12_datatype, arg12_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype,arg11_datatype,arg12_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnCurrent) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value,arg11_value,arg12_value)
#define EF_OM_12(class_index, vmethod_index, ret_datatype, pThis, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value, arg9_datatype, arg9_value, arg10_datatype, arg10_value, arg11_datatype, arg11_value, arg12_datatype, arg12_value)    \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype,arg7_datatype,arg8_datatype,arg9_datatype,arg10_datatype,arg11_datatype,arg12_datatype)) (pThis)->m_pObjectInfo->m_aryInfos [class_index].m_pVirtualFuncTable [vmethod_index].m_pfnOld) (pThis, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value,arg7_value,arg8_value,arg9_value,arg10_value,arg11_value,arg12_value)

//////////////////////////////////////////////////////////////////////////
// EF_SM_* 用于调用对象的静态方法
// EF_SM_* 参数中的method_index为方法在所有类方法（包括静态、非静态以及私有方法）中的索引(>=0)

#define _EF_CLASSINFO(class_name)  ((EFClassInfo*) sys_Runtime_LoadClassInfo(class_name))

#define EF_SM_0(class_name, method_index, ret_datatype)    \
	(*(ret_datatype (*) ()) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) ();
#define EF_SM_1(class_name, method_index, ret_datatype, arg1_datatype, arg1_value)    \
	(*(ret_datatype (*) (arg1_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value);
#define EF_SM_2(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value);
#define EF_SM_3(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value);
#define EF_SM_4(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value);
#define EF_SM_5(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value);
#define EF_SM_6(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value);
#define EF_SM_7(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype, arg7_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value, arg7_value);
#define EF_SM_8(class_name, method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value)    \
	(*(ret_datatype (*) (arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype, arg7_datatype, arg8_datatype)) _EF_CLASSINFO(class_name)->m_pMethodTable[method_index].m_func) (arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value, arg7_value, arg8_value);

//////////////////////////////////////////////////////////////////////////
// EF_IM_* 用于调用接口的方法
// EF_IM_* 参数中的interface_method_index为方法在指定接口所有方法(连同所有基接口方法!)中的方法索引(>=0)

#define EF_IM_0(pInterface, interface_method_index, ret_datatype)  \
	(*(ret_datatype (*) (EFObject*)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject);
#define EF_IM_1(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value);
#define EF_IM_2(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value);
#define EF_IM_3(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value);
#define EF_IM_4(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value, arg4_value);
#define EF_IM_5(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value);
#define EF_IM_6(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value);
#define EF_IM_7(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype, arg7_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value, arg7_value);
#define EF_IM_8(pInterface, interface_method_index, ret_datatype, arg1_datatype, arg1_value, arg2_datatype, arg2_value, arg3_datatype, arg3_value, arg4_datatype, arg4_value, arg5_datatype, arg5_value, arg6_datatype, arg6_value, arg7_datatype, arg7_value, arg8_datatype, arg8_value)  \
	(*(ret_datatype (*) (EFObject*, arg1_datatype, arg2_datatype, arg3_datatype, arg4_datatype, arg5_datatype, arg6_datatype, arg7_datatype, arg8_datatype)) (pInterface)->m_pInterfaceFuncs[interface_method_index].m_pfnCurrent) ((pInterface)->m_pObject, arg1_value, arg2_value, arg3_value, arg4_value, arg5_value, arg6_value, arg7_value, arg8_value);

//////////////////////////////////////////////////////////////////////////

// 返回指定类的成员数据起始地址
#define EF_GET_OBJECT_FIELDS_DATA_ADDRESS(pThis,class_index)   ((pThis)->m_paryData [class_index])

#define EF_GET_CLASS_COUNT(pObject)  \
	(((EFObject*)(pObject))->m_pObjectInfo->m_nClassCount)

#define EF_ARRAY_OF(datatype)   EF_ARRAY_DATA_TYPE_LEADER_TEXT datatype

//////////////////////////////////////////////////////////////////////////

// 定义多返回值时候使用的宏
#define	EF_TYPES_2(t1,t2)              t1 myT(",") t2
#define	EF_TYPES_3(t1,t2,t3)           t1 myT(",") t2 myT(",") t3
#define EF_TYPES_4(t1,t2,t3,t4)        t1 myT(",") t2 myT(",") t3 myT(",") t4
#define EF_TYPES_5(t1,t2,t3,t4,t5)     t1 myT(",") t2 myT(",") t3 myT(",") t4 myT(",") t5
#define EF_TYPES_6(t1,t2,t3,t4,t5,t6)  t1 myT(",") t2 myT(",") t3 myT(",") t4 myT(",") t5 myT(",") t6

//////////////////////////////////////////////////////////////////////////
// GC相关

#include <eflib/sys/object.h>
#include <eflib/sys/runtime.h>
#include <eflib/sys/event.h>

// 创建文本/字节集/对象/接口，详见下文说明
#define EF_NEW_TEXT(text)          EF_GC_CLONE_AND_REG_TEXT_DATA(text)
#define EF_NEW_BIN(bin, len)       EF_GC_CLONE_AND_REG_BIN_DATA(bin, len)
#define EF_NEW_OBJECT(class_name)     sys_Runtime_NewObject2(class_name, EF_TRUE)   // 调用默认初始化方法
#define EF_NEW_OBJECT_NC(class_name)  sys_Runtime_NewObject2(class_name, EF_FALSE)  // 不调用默认初始化方法
#define EF_NEW_INTERFACE(object, interface_name)  sys_Runtime_CreateInterface(object, interface_name)

// 创建指定数据类型的对象实体并将其登记到GC管理器中, 其引用计数将被初始化为1.
// 成功返回EFObject*, 失败返回NULL.
// EF_GC_CREATE_OBJECT()参数为易语言文本格式(EFText)，EF_GC_CREATE_OBJECT2()参数为C语言格式(EFChar*)
#define EF_GC_CREATE_OBJECT(class_name)   sys_Runtime_NewObject(class_name, EF_TRUE)
#define EF_GC_CREATE_OBJECT2(class_name)  sys_Runtime_NewObject2(class_name, EF_TRUE)

// 将指定EFChar*文本数据复制后登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向新数据的指针(EFText)。
#define EF_GC_CLONE_AND_REG_TEXT_DATA(text) ((EFText)sys_GC_CloneAndRegTextData(text))

// 将指定数据(void*)复制后登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向新数据的指针(EFBinary)。
#define EF_GC_CLONE_AND_REG_BIN_DATA(bin, len)  ((EFBinary)sys_GC_CloneAndRegBinData(bin, len))

// 将指定EFText格式文本数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFText)。
#define EF_GC_REG_TEXT_DATA(eftext_data)  ((EFText)sys_GC_RegisterData(eftext_data, EF_GCDT_PUREDATA))

// 将指定EFBinary格式数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFBinary)。
#define EF_GC_REG_BIN_DATA(efbin_data)  ((EFBinary)sys_GC_RegisterData(efbin_data, EF_GCDT_PUREDATA))

// 将指定EFObject格式数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFObject*)。
#define EF_GC_REG_OBJECT_DATA(ef_object_data_ptr)  ((EFObject*)sys_GC_RegisterData(ef_object_data_ptr, EF_GCDT_OBJECT))

// 将指定EFInterface格式数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFInterface*)。
#define EF_GC_REG_INTERFACE_DATA(ef_interface_data_ptr)  ((EFInterface*)sys_GC_RegisterData(ef_interface_data_ptr, EF_GCDT_INTERFACE))

// 将指定EFArray格式非对象、文本或字节集数组数据登记到GC管理器中,  其引用计数将被初始化为1。
// 返回指向该数据的指针(EARRAY)。
#define EF_GC_REG_PUREDATA_ARRAY_DATA(efarray_data)  ((EFArray)sys_GC_RegisterData(efarray_data, EF_GCDT_PUREDATA))

// 将指定EFArray格式对象数组数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFArray)。
// 注意!! 登记时marray_data数据必须完整有效，否则有可能导致GC管理器异常。
#define EF_GC_REG_OBJECT_ARRAY_DATA(efarray_data)  ((EFArray)sys_GC_RegisterData(efarray_data, EF_GCDT_OBJECT_ARRAY))

// 将指定EFArray格式接口数组数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFArray)。
// 注意!! 登记时marray_data数据必须完整有效，否则有可能导致GC管理器异常。
#define EF_GC_REG_INTERFACE_ARRAY_DATA(efarray_data)  ((EFArray)sys_GC_RegisterData(efarray_data, EF_GCDT_INTERFACE_ARRAY))

// 将指定EFArray格式文本或字节集数组数据登记到GC管理器中, 其引用计数将被初始化为1。
// 返回指向该数据的指针(EFArray)。
// 注意!! 登记时marray_data数据必须完整有效，否则有可能导致GC管理器异常。
#define EF_GC_REG_TEXT_OR_BIN_ARRAY_DATA(efarray_data)  ((EFArray)sys_GC_RegisterData(efarray_data, EF_GCDT_TEXT_OR_BIN_ARRAY))
#define EF_GC_REG_TEXT_ARRAY_DATA(efarray_data)  EF_GC_REG_TEXT_OR_BIN_ARRAY_DATA(efarray_data)
#define EF_GC_REG_BIN_ARRAY_DATA(efarray_data)   EF_GC_REG_TEXT_OR_BIN_ARRAY_DATA(efarray_data)

// 增加指定受控数据（已注册到GC）的被引用计数, 并直接返回参数。
#define EF_INC_REF_COUNT(mdata)  sys_GC_IncRefCount(mdata)
// 减少指定受控数据（已注册到GC）的被引用计数。如果被引用计数达到0，则该数据将被GC删除。
#define EF_DEC_REF_COUNT(mdata)  sys_GC_DecRefCount(mdata)


//负责减小受控数据块的引用计数。使用本对象，即使代码中途产生异常，也可确保正确地减小引用计数。
class EF_RefCountDecer
{
private:
	void* m_pData;
public:
	EF_RefCountDecer(void* pData) : m_pData(pData)
	{
	}

	~EF_RefCountDecer()
	{
		EF_DEC_REF_COUNT(m_pData);
	}
};

#endif //__EF_H__
