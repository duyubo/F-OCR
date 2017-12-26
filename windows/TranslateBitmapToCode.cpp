// TranslateBitmapToCode.cpp : 定义 DLL 应用程序的导出函数。
//

#include "TranslateBitmapToCode.h"
#include "Matrix.h"
#include "recognize.h"
#include "logical_relation_and_scriptchanging.h"

#ifdef USE_MY_QSORT
void quicksort(byte *start, byte *end)	//'end' is just out of the array
{
	if (start - end <= 1)
		return;
	byte *head = start, *rear = end - 1;
	byte key = *start;
	bool raising = false;
	while (head < rear)
	{
		if (raising)
		{
			if (*head > key)
			{
				*rear-- = *head;
				raising = false;
			}
			else
				head++;
		}
		else
		{
			if (*rear < key)
			{
				*head++ = *rear;
				raising = true;
			}
			else
				rear--;
		}
	}
	*head = key;
	quicksort(start, head);
	quicksort(head + 1, end);
}
#else
int compare(const void *arg1, const void *arg2)
{
	return *(byte*)arg1 - *(byte*)arg2;
}

#endif	//USE_MY_QSORT

void Binaryzation(byte *map, int size)
{
	//类间方差g=w1w2(u1-u2)^2
	//w1、w2:权重
	//u1、u2:均值

	//排序
	//第一趟排序，顺便复制数组
	byte* arr = new byte[size];
	byte key = *map;
	byte *head = arr, *rear = arr + size, *mapEnd = map + size;
	bool increasing = false;

#ifdef USE_MY_QSORT
	for (byte* p = map + 1; p < mapEnd; p++)
	{
		if (*p < key)
			*(head++) = *p;
		else
			*--rear = *p;
	}
	*head = key;

	//第一趟排序完成
	quicksort(arr, head);
	quicksort(head + 1, arr + size);
	//排序完成
#else
	for (int i = 0; i < size; i++)
	{
		arr[i] = map[i];
	}
	qsort(arr, size, sizeof(byte), compare);
#endif	//USE_MY_QSORT

	//sum[k]=sum of arr[i](i = 1 to k)
	long *sum = new long[size];
	long _sum = 0;
	for (int i = 0; i < size; i++)
		sum[i] = (_sum += arr[i]);
	//sum[]构造完成

	double g, gmax = 0;
	byte mid;	//阈值
	for (int i = 0; i < size; i++)
	{
		while (i + 1 < size && arr[i + 1] == arr[i])
			i++;
		double u1 = sum[i] / (i + 1);
		double u2 = (_sum - sum[i]) / (size - i);
		double du = u1 - u2;
		g = (i + 1) * (size - i) * du * du;
		if (g > gmax)
		{
			gmax = g;
			mid = arr[i];
		}
	}
	///*强行*/mid = 0xf0;
	for (byte* p = map; p < mapEnd; p++)
	{
		*p = *p > mid ? 0 : 1;
	}

	delete[] sum;
	delete[] arr;
}

//从HBITMAP转成Matrix
//要求位图格式为24bppRGB
//size将被写入图片大小
byte *BitmapToMatrix(BITMAP *bitmap)
{
	int matrixSize = bitmap->bmWidth * bitmap->bmHeight;
	int bmpBitSize = bitmap->bmWidthBytes * bitmap->bmHeight;
	int _stride = bitmap->bmBitsPixel / 8;
	byte *Values = (byte *)bitmap->bmBits;
	byte *_matrix = new byte[matrixSize];
	int iBitmap0 = bitmap->bmWidthBytes*(bitmap->bmHeight - 1), iBitmap, iMatrix;
	if (bitmap->bmWidthBytes > 0)
	{
		iMatrix = 0;
		for (int y = 0; y < bitmap->bmHeight; y++)
		{
			iBitmap = iBitmap0;
			iBitmap0 -= bitmap->bmWidthBytes;
			for (int x = 0; x < bitmap->bmWidth; x++)
			{
				_matrix[iMatrix] = (byte)(Values[iBitmap] * 0.114 +
					Values[iBitmap+1] * 0.587 + Values[iBitmap+2] * 0.299);
				iBitmap += _stride;
				iMatrix++;
			}
		}
	}
	else
	{
		//哪个孙子搞出来的鬼格式
		for (int y = 0; y < bitmap->bmHeight; y++)
		{
			iMatrix = matrixSize;
			iBitmap = iBitmap0;
			iBitmap0 -= bitmap->bmWidthBytes;
			for (int x = 0; x < bitmap->bmWidth; x++)
			{
				iMatrix--;
				_matrix[iMatrix] = (byte)(Values[iBitmap] * 0.114 +
					Values[iBitmap] * 0.587 + Values[iBitmap] * 0.299);
				iBitmap += 3;
			}
		}
	}

	return _matrix;
}

//recognize reg;
inline void initRecognize()
{
	//recognize::load_sample("C:\\Users\\Crazylqx\\Desktop\\rn.txt");
	//recognize::create_sample_file("training_data.dat","training_data.txt");
	recognize::load_sample("training_data.dat");
}

inline void initLogical()
{
	if (!Logical::load_file())
	{
		MessageBox(NULL, TEXT("加载文件错误"), TEXT(""), MB_OK);
	}
}

void InitDll(void)
{
#ifndef LQX_ONLY
	initRecognize();
	initLogical();
#endif // !LQX_ONLY
}

std::string *TranslateLine(MatrixList *mtrxList);


//#define WRITE_MATRIX_TO_FILE

//无误则返回0
C_TBTC_API std::string* __cdecl TranslateBitmapToCode(HBITMAP hBitmap, INT32 &nCount, INT32 &ErrorCode)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	byte *map = BitmapToMatrix(&bitmap);
	Binaryzation(map, bitmap.bmWidth*bitmap.bmHeight);
	Matrix matrix(bitmap.bmWidth, bitmap.bmHeight, map);
	auto lines = matrix.SeperateToLines();
	//Text to Output.
	std::string *text=new std::string();
	for (auto i = lines->begin(); i < lines->end(); i++)
	{
		std::string *_line = TranslateLine(*i);
		*text += *_line;
		delete _line;
	}
	delete lines;

	nCount = text->length();
	ErrorCode = 0;

#ifdef _DEBUG
	return text;
#else
#error 未定义返回值
	return 0;
#endif // _DEBUG
}

C_TBTC_API void __cdecl GetString(std::string *strPtr, char *buffer)
{
	const char *str = strPtr->c_str();
	while (*str)
	{
		*(buffer++) = *(str++);
	}
	*buffer = 0;
	delete strPtr;
}

#ifdef _DEBUG
void debug_log(Logical::inf &i)
{
	FILE *fp;
	if (fopen_s(&fp, "E:\\inf.txt", "a+"))
		exit(-1);
	fprintf(fp, "%d %d %d %d %d\n", i.order, i.w, i.l, i.x, i.y);
	fclose(fp);
}
#else
#define debug_log(X)
#endif // _DEBUG

std::string *TranslateLine(MatrixList *mtrxList)
{
	//MatrixList *mtrxList = matrix->Seperate();
	for (MatrixList::iterator i = mtrxList->begin(); i < mtrxList->end(); i++)
	{
		i->width = i->matrix->GetWidth();
		i->height = i->matrix->GetHeight();
		i->matrix->ToStdSquare();
#ifndef LQX_ONLY
		i->GetCharactor();
#endif // 
	}

#ifdef WRITE_MATRIX_TO_FILE
	WCHAR msg[256];
	wsprintf(msg, TEXT("子矩阵个数：%d"), mtrxList->size());
	MessageBox(NULL, msg, TEXT(""), MB_OK);
	const char filename[] = "E:\\fuck.txt";
	FILE *fp;
	if (fopen_s(&fp, filename, "w"))
	{
		MessageBox(NULL, TEXT("Can not open file."), TEXT(""), MB_ICONERROR);
		return FALSE;
	}
	for (MatrixList::iterator i = mtrxList->begin(); i < mtrxList->end(); i++)
	{
		i->GetCharactor();
		fprintf(fp, "%d\t%d\t%d\t%d\t%d\n", i->charactor, i->width, i->height, i->x, i->y);
		for (size_t j = 0; j < Matrix::StdHeight*Matrix::StdWidth; j++)
		{
			fprintf(fp, "%d ", (int)(i->matrix->map[j]));
		}
		fprintf(fp, "\n");
		
	}
	fclose(fp);
	MessageBox(NULL, TEXT("Finished"), TEXT(""), MB_OK);
#else

#ifndef _DEBUG
#error 这里可以优化
#endif

#ifdef LQX_ONLY
	std::string *resultString = new std::string("Result");
#else
	std::vector<Logical::inf> set;
#ifdef _DEBUG
	FILE *fp;
	fopen_s(&fp, "inf.log", "w");
#endif
	for (MatrixList::iterator i = mtrxList->begin(); i < mtrxList->end(); i++)
	{
		Logical::inf information(i->charactor, i->width, i->height, i->x, i->y);
		debug_log(information);
		set.push_back(information);
#ifdef _DEBUG
		fprintf(fp, "%d %d %d %d %d\n", i->charactor, i->width, i->height, i->x, i->y);
#endif
	}
#ifdef _DEBUG
	fclose(fp);
#endif

	std::string *resultString = new std::string();
 	Logical::change(set, *resultString);
#endif // LQX_ONLY

#endif

	//后续代码
#ifndef _DEBUG
#error 代码未完成
#endif
	deleteMatrixList(mtrxList);
	
	return resultString;
}

#ifdef _DEBUG

C_TBTC_API INT32 __cdecl FuckBitmap(HBITMAP hBitmap,const char * filename, INT32 number)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	byte *map = BitmapToMatrix(&bitmap);
	Binaryzation(map, bitmap.bmWidth*bitmap.bmHeight);
	Matrix matrix(bitmap.bmWidth, bitmap.bmHeight, map);
	MatrixList *mtrxList = matrix.Seperate();
//	int xx = mtrxList->size() - 1;
//	if (mtrxList->size() != 1)
//	{
//		MessageBox(NULL, TEXT("Uh oh"), TEXT(""), MB_ICONERROR);
//		return FALSE;
//	}
	FILE *fp;
	if (fopen_s(&fp, filename, "a+"))
	{
		MessageBox(NULL, TEXT("Can not open file."), TEXT(""), MB_ICONERROR);
			return FALSE;
	}
	int msz = mtrxList->size();
	if (msz == 0)
	{
		MessageBox(NULL, TEXT("Nothing inside."), TEXT(""), MB_ICONERROR);
		return FALSE;
	}
	msz--;
	fprintf(fp, "%d %d %d %d %d\n",
		number, (*mtrxList)[msz].x, (*mtrxList)[msz].y, (*mtrxList)[msz].matrix->GetWidth(), (*mtrxList)[msz].matrix->GetHeight());
/*	(*mtrxList)[msz].matrix->ToStdSquare();
	byte *_map = (*mtrxList)[msz].matrix->map;
	for (int i = 0; i < Matrix::StdWidth * Matrix::StdHeight; i++)
	{
		fprintf(fp, "%d ", (int)_map[i]);
	}
	fprintf(fp, "%d\n", number);
	*/
	fclose(fp);
	deleteMatrixList(mtrxList);
	return TRUE;
}

/*
void MatrixToBitmap(BITMAP *bitmap, byte *map)
{
		int matrixSize = bitmap->bmWidth * bitmap->bmHeight;
		int bmpBitSize = bitmap->bmWidthBytes * bitmap->bmHeight;
		int _stride = bitmap->bmBitsPixel / 8;
		byte *Values = (byte *)bitmap->bmBits;
	//	byte *_matrix = new byte[matrixSize];
		int iBitmap0 = bitmap->bmWidthBytes*(bitmap->bmHeight - 1), iBitmap, iMatrix;
		if (bitmap->bmWidthBytes > 0)
		{
			iMatrix = 0;
			for (int y = 0; y < bitmap->bmHeight; y++)
			{
				iBitmap = iBitmap0;
				iBitmap0 -= bitmap->bmWidthBytes;
				for (int x = 0; x < bitmap->bmWidth; x++)
				{
	//				_matrix[iMatrix] = (byte)(Values[iBitmap] * 0.114 +
	//					Values[iBitmap + 1] * 0.587 + Values[iBitmap + 2] * 0.299);
					if (map[iMatrix])
					{
						Values[iBitmap] = Values[iBitmap + 1] = Values[iBitmap + 2] = 0;
					}
					else
					{
						Values[iBitmap] = Values[iBitmap + 1] = Values[iBitmap + 2] = 255;
					}
					iBitmap += _stride;
					iMatrix++;
				}
			}
		}

}
*/

MatrixList *_global_matrix_list;
C_TBTC_API INT32 __cdecl SeperateBitmapToStdMatrix(HBITMAP hBitmap)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	byte *map = BitmapToMatrix(&bitmap);
	Binaryzation(map, bitmap.bmWidth*bitmap.bmHeight);
	Matrix matrix(bitmap.bmWidth, bitmap.bmHeight, map);
	_global_matrix_list = matrix.Seperate();
	for (auto i = _global_matrix_list->begin(); i < _global_matrix_list->end(); i++)
	{
		i->width = i->matrix->GetWidth();
		i->height = i->matrix->GetHeight();
		i->matrix->ToStdSquare();
	}
	return _global_matrix_list->size();
}

C_TBTC_API byte* __cdecl GetStdMatrix(INT32 n, INT32 &x, INT32 &y, INT32 &width, INT32 &height)
{
	_matrix_list_node &mln = (*_global_matrix_list)[n];
	x = mln.x;
	y = mln.y;
	width = mln.width;
	height = mln.height;
	return mln.matrix->map;
}

C_TBTC_API void __cdecl FreeMatrixList()
{
	deleteMatrixList(_global_matrix_list);
}

#endif //_DEBUG



