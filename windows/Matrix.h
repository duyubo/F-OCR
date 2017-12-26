#pragma once
#include <vector>
#include "recognize.h"

struct _matrix_list_node;
typedef std::vector<_matrix_list_node> MatrixList;
typedef unsigned char byte;

#include "TranslateBitmapToCode.h"
//二值化
C_TBTC_API void __cdecl Binaryzation(byte *map, int size);

class Matrix
{
public:
	Matrix(int width, int height, byte* map);
	Matrix(int width, int height);
	~Matrix();
	byte GetPoint(int x, int y);
	void SetPoint(int x, int y);
	int GetWidth();
	int GetHeight();
	//将图片进行分割
	//内存不足时返回nullptr
	MatrixList *Seperate();
	//变换为标准矩形
	void ToStdSquare();
	//分割为多行
	std::vector<MatrixList*> *SeperateToLines();

public:
	static const int StdWidth = 28;
	static const int StdHeight = 28;
	byte *map;

private:
	int width;
	int height;
};

//分数线（减号）的字符编号
const int StraightLine = 82;

struct _matrix_list_node
{
	int x;
	int y;
	int width;
	int height;
	Matrix *matrix;
	int charactor;
	void GetCharactor();
};

inline void deleteMatrixList(MatrixList *mtrxlist)
{
	for (MatrixList::iterator i = mtrxlist->begin(); i < mtrxlist->end(); i++)
	{
		delete i->matrix;
	}
	delete mtrxlist;
}

inline void _matrix_list_node::GetCharactor()
{
	charactor = recognize::GetIndex(matrix->map, COSINE);
}

inline Matrix::Matrix(int width, int height, byte * map)
	:width(width), height(height), map(map) {}


inline Matrix::Matrix(int width, int height)
	: width(width), height(height)
{
	int size = width*height;
	map = new byte[size];
	for (int i = 0; i < size; i++)
	{
		map[i] = 0;
	}
}

inline Matrix::~Matrix()
{
	delete[] map;
}

inline byte Matrix::GetPoint(int x, int y)
{
	return map[width*y + x];
}

inline void Matrix::SetPoint(int x, int y)
{
	map[x + y * width] = 1;
}

inline int Matrix::GetWidth()
{
	return width;
}

inline int Matrix::GetHeight()
{
	return height;
}
