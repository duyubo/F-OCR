// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TRANSLATEBITMAPTOCODE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TRANSLATEBITMAPTOCODE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
#error ��û������ͼƬ��С
#endif