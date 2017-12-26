// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TRANSLATEBITMAPTOCODE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TRANSLATEBITMAPTOCODE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#define TRANSLATEBITMAPTOCODE_EXPORTS

#ifdef TRANSLATEBITMAPTOCODE_EXPORTS
#define TRANSLATEBITMAPTOCODE_API __declspec(dllexport)
#else
#define TRANSLATEBITMAPTOCODE_API //__declspec(dllimport)
#endif

#include<Windows.h>
#include<string>

#define C_TBTC_API extern"C" TRANSLATEBITMAPTOCODE_API

C_TBTC_API std::string* __cdecl TranslateBitmapToCode(HBITMAP hBitmap, INT32 &nCount, INT32 &ErrorCode);

//#define LQX_ONLY

#ifndef _DEBUG
#error 还没有限制图片大小
#endif