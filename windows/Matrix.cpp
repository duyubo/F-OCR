#include "Matrix.h"

#ifdef _DEBUG
inline void assert(bool x)
{
	if (x == false)
		MessageBox(NULL, TEXT("ASSERT_FAULT"), TEXT("ERROR"), MB_ICONERROR);
}
#else
#define assert(X)
#endif


MatrixList * Matrix::Seperate()
{
	MatrixList *list = new MatrixList;
	int size = width * height;
	//复制map，产生副本tempMap
	byte *tempMap = new byte[size];
	if (tempMap == nullptr)
		return nullptr;

	for (int i = 0; i < size; i++)
	{
		tempMap[i] = map[i];
	}
	//创建一个队列，存储读取的点
	struct point
	{
		int x, y;
	};
	point *_list = new point[size];
	point *_head;
	point *_rear;
	if (_list == nullptr)
	{
		delete[] tempMap;
		return nullptr;
	}
	//遍历tempMap
	for (int i = 0; i < size; i++)
	{
		if (tempMap[i])   
		{
			_head = _list;
			_rear = _list;
			tempMap[i] = 0;
			_head->x = i % width;
			_head->y = i / width;
			_head++;
			int x1, x2, y1, y2;
			x1 = x2 = i % width;
			y1 = y2 = i / width;
			while (_head > _rear)
			{
				assert(_head <= _list + size);
				int _x = _rear->x;
				int _y = _rear->y;
				//扩充边界范围
				{
					if (_x > x2)
						x2 = _x;
					if (_x < x1)
						x1 = _x;
					if (_y > y2)
						y2 = _y;
					if (_y < y1)
						y1 = _y;
				}
				_rear++;

				int tx, ty;

#define ADD_NODE(X,Y)													\
				tx=X;	ty=Y;											\
				if(tempMap[ty * width + tx] && tx >= 0 && tx < width && ty >= 0 && ty < height){	\
					assert(ty * width + tx < size);						\
					assert(ty * width + tx >= 0);						\
					tempMap[ty * width + tx] = 0;						\
					assert(_head - _list < size);						\
					_head->x = tx;										\
					_head->y = ty;										\
					_head++;											\
				}
				

				ADD_NODE(_x - 1, _y);
				ADD_NODE(_x + 1, _y);
				ADD_NODE(_x, _y - 1);
				ADD_NODE(_x, _y + 1);
#ifndef STRICTLY_SEPERATE
				ADD_NODE(_x - 1, _y - 1);
				ADD_NODE(_x + 1, _y - 1);
				ADD_NODE(_x - 1, _y + 1);
				ADD_NODE(_x + 1, _y + 1);
#endif // !STRICTLY_SEPERATE
			}
			//创建一个Matrix;
			assert(x2 >= x1);
			assert(y2 >= y1);

			Matrix *pmtrx = new Matrix(x2 - x1 + 1, y2 - y1 + 1);
			for (point* p = _list; p < _head; p++)
			{
				assert(p->x >= x1 && p->y >= y1 && p->x <= x2 && p->y <= y2);
				pmtrx->SetPoint(p->x-x1, p->y-y1);
			}
			//在list中添加节点
			_matrix_list_node nd;
			nd.x = (x2 + x1) / 2;
			nd.y = (y2 + y1) / 2;
			nd.matrix = pmtrx;
			list->push_back(nd);

		}
	}
	delete[] tempMap;
	delete[] _list;
	return list;
}

void Matrix::ToStdSquare()
{
	int len;
	int stdwsize = StdWidth * height;
	double wx = (double)StdWidth / width;
	double hx = (double)StdHeight / height;
	double xx = hx > wx ? wx : hx;

	byte *stdwMap = new byte[stdwsize];
	byte *stdwMapEnd = stdwMap + stdwsize;
	for (byte *p = stdwMap; p < stdwMapEnd; p++)
		*p = 0;
	byte *ml = map;
	byte *stdml = stdwMap;
	for (int y = 0; y<height; y++, ml += width, stdml += StdWidth)
	{
		bool fuck = false;
		len = 0;
		for (int x = 0; x < width; x++)
		{
			if (ml[x])
			{
				fuck = true;
				len++;
			}
			else if (fuck)
			{
				fuck = false;
				int stdx = (x - len)*xx;
				//len *= xx;
				len = ceil(len * xx);
		//		if (len == 0)
		//			len = 1;
				for (int dx = 0; dx<len; dx++)
				{
					assert(&stdml[stdx] < stdwMapEnd);
					stdml[stdx++] = 1;
				}
				len = 0;
			}
		}
		if (fuck)
		{
			int stdx = (width - len)*xx;
			len = ceil(len * xx);
		//	if (len == 0)
		//		len = 1;
			for (int dx = 0; dx < len && stdx < StdWidth; dx++)
			{
				assert(&stdml[stdx] < stdwMapEnd);
				stdml[stdx++] = 1;
			}
		}
	}

	byte *stdMap = new byte[StdWidth*StdHeight];
	byte *stdMapEnd = stdMap + StdWidth*StdHeight;
	for (byte *p = stdMap; p < stdMapEnd; p++)
		*p = 0;
	for (int x = 0; x < StdWidth; x++)
	{
		bool fuck = false;
		byte *p = stdwMap + x;
		len = 0;
		for (int y = 0; p < stdwMapEnd; y++, p += StdWidth)
		{
			if (*p)
			{
				fuck = true;
				len++;
			}
			else if(fuck)
			{
				fuck = false;
				int stdy = (y - len) * xx;
				byte *stdp = stdMap + x + stdy*StdWidth;
				//len *= xx;
				len = ceil(len * xx);
		//		if (len == 0)
		//			len = 1;
				for (int dy = 0; dy < len && stdp < stdMap + StdWidth * StdHeight; dy++, stdp += StdWidth)
				{
					assert(stdp < stdMap + StdWidth*StdHeight);
					*stdp = 1;
				}
				len = 0;
			}
		}
		if (fuck)
		{
			int stdy = (height - len)*xx;
			byte *stdp = stdMap + x + stdy*StdWidth;
			//len *= xx;
			len = ceil(len * xx);
		//	if (len == 0)
		//		len = 1;
			for (int dy = 0; dy < len; dy++, stdp += StdWidth)
			{
				assert(stdp < stdMap + StdWidth*StdHeight);
				*stdp = 1;
			}
		}
	}

	delete[] map;
	delete[] stdwMap;
	map = stdMap;
	width = StdWidth;
	height = StdHeight;
}

std::vector<MatrixList*> * Matrix::SeperateToLines()
{
	auto list = new std::vector<MatrixList*>();
	list->push_back(Seperate());
	return list;
	/*
	int size = width*height;
	byte *p = map;
	bool got = false;
	int last_y;
	for (int y = 0; y < height; y++)
	{
		int dark = 0;
		for (int x = 0; x < width; x++, p++)
		{
			dark += *p;
		}
		if (dark)
		{
			if (!got)
			{
				got = true;
				last_y = y;
			}
		}
		else
		{
			if (got)
			{
				int h = y - last_y;
				byte *_start = map + last_y * width;
				int sz = width * h;
				byte *mp = new byte[sz];
				for (int i = 0; i < sz; i++)
				{
					mp[i] = _start[i];
				}
				Matrix *thisLine = new Matrix(width, h, mp);
				MatrixList *ml = thisLine->Seperate();
				delete thisLine;
				if (ml->size()==1)
				{
					(*ml)[0].GetCharactor();
					if ((*ml)[0].charactor == StraightLine)
					{
						continue;
					}
				}
				list->push_back(ml);
			}
			got = false;
		}
	}
	return list;
	*/
}

/*
void Matrix::OldToStdSquare()
{
	double *wstdmap = new double[StdWidth * height];
	double a, b = 0;
	int k1, k2 = 0;
	double xx = (double)width / StdWidth;
	double halfxx = xx / 2;
#ifndef _DEBUG
#error 下面的代码有待优化
#endif // !_DEBUG

	for (int i = 0; i < StdWidth; i++)
	{
		a = b;
		b = (i + 1) * xx;
		k1 = k2;
		k2 = (int)b;
		b -= k2;
		for (int y = 0; y < height; y++)
		{
			double v = map[y*width + k2] * b - map[y*width + k1] * a;
			for (int j = k2 - 1; j >= k1; j--)
			{
				v += map[y*width + j];
			}
			wstdmap[y*StdWidth + i] = v / xx;
		}
	}


	byte *stdmap = new byte[StdWidth*StdHeight];
	b = 0;
	xx = (double)height / StdHeight;
	halfxx = xx / 2;
	k2 = 0;
	for (int i = 0; i < StdHeight; i++)
	{
		a = b;
		b = (i + 1) * xx;
		k1 = k2;
		k2 = (int)b;
		b -= k2;
		for (int x = 0; x < StdWidth; x++)
		{
			double v = map[x + StdWidth * k2] * b - wstdmap[x + StdWidth * k1] * a;
			for (int j = k2 - 1; j >= k1; j--)
			{
				v += wstdmap[x + StdWidth * j];
			}
			stdmap[x + StdWidth * i] = (byte)(v >= halfxx);
		}
	}

	delete[] map;
	delete[] wstdmap;
	map = stdmap;
	width = StdWidth;
	height = StdHeight;
}

*/

