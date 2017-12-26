#include "recognize.h"
#include <iostream>
#include <math.h>
#include <exception>
#ifdef LOAD_SAMPLE_FROM_FILE

std::vector<sample> recognize::all_sample;


#if defined _WINODOWS || _WIN32 || _WIN64
#include <Windows.h>
#define THROW_EXCEPTION(MSG) _throw_exception(TEXT(MSG)) 

void _throw_exception(TCHAR* msg)
{
	MessageBox(NULL, msg, TEXT(""), MB_OK);
	exit(-1);
}
#else
#define THROW_EXCEPTION(MSG)
#endif // _WINDOWS

inline int getFileLength(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	int value = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return value;
}


inline bool isBlank(int c)
{
	switch (c)
	{
	case ' ':
	case '\t':
	case '\n':
	case '\r': 
		return true;
	default:
		return false;
	}
}

inline int getNumber(char *&p)
{
	int value;
	if (sscanf_s(p, "%d", &value) == 0)
		return -1;
	while (iswdigit(*p))
		p++;
	while (isBlank(*p))
		p++;
	return value;
}

#ifdef _DEBUG

void recognize::create_sample_file(char *dest, char *src)
{
	FILE *fdest, *fsrc;
	int n = 0;
	if (fopen_s(&fdest, dest, "wb") || fopen_s(&fsrc, src, "r"))
	{
		THROW_EXCEPTION("未能打开文件");
		return;
	}
	int fileLength = getFileLength(fsrc);
	char *buffer = new char[fileLength];
	fileLength = fread(buffer, 1, fileLength, fsrc);
	fclose(fsrc);
	fwrite(&n, sizeof(n), 1, fdest);
	char *p = buffer;
	char *bufferEnd = buffer + fileLength;
	while (p < bufferEnd)
	{
		sample sp;
		for (size_t i = 0; i < length_of_vector + 2 * normal_length; i++)
		{
			int value = getNumber(p);
			if (value == -1)
			{
				if (i)
					THROW_EXCEPTION("Error, can not get number.");
				else
					return;
			}
			sp.vec[i] = value;
		}
		if ((sp.index = getNumber(p)) == -1)
			THROW_EXCEPTION("Error, can not get number.");
		fwrite(&sp, sizeof(sp), 1, fdest);
		n++;
	}
	delete[] buffer;
		int fk = ftell(fdest);
	fseek(fdest, 0, SEEK_SET);
	fwrite(&n, sizeof(n), 1, fdest);
	fclose(fdest);
	
}

void recognize::load_sample_from_txtfile(char *sampleFileName)
{
	FILE *fp;
	if (fopen_s(&fp, sampleFileName, "r"))
		THROW_EXCEPTION("Can not open sample file.");
	int fileLength = getFileLength(fp);
	char *buffer = new char[fileLength];
	fileLength = fread(buffer, 1, fileLength, fp);
	fclose(fp);
	char *p = buffer;
	char *bufferEnd = buffer + fileLength;
	while (p < bufferEnd)
	{
		sample sp;
		for (size_t i = 0; i < length_of_vector + 2 * normal_length; i++)
		{
			int value = getNumber(p);
			if (value == -1)
			{
				if (i)
					THROW_EXCEPTION("Error, can not get number.");
				else
					return;
			}
			sp.vec[i] = value;
		}
		if ((sp.index = getNumber(p)) == -1)
			THROW_EXCEPTION("Error, can not get number.");
		all_sample.push_back(sp);
	}
	delete[] buffer;
}

#endif

void recognize::load_sample(char *sampleFileName)
{
	int n;
	FILE *fp;
	if (fopen_s(&fp, sampleFileName, "rb"))
	{
		THROW_EXCEPTION("无法加载文件");
	}
	fread(&n, sizeof(n), 1, fp);
	all_sample.resize(n);
	fread(&(all_sample[0]), sizeof(sample), n, fp);
	fclose(fp);
}

#endif // LOAD_SAMPLE_FROM_TXTFILE

int recognize::GetIndex(byte *vec, int type_of_distande)
{
	double low_vec[length_of_vector];
	double row_col[normal_length * 2];
	double vec_d[normal_length*normal_length];
	double feature_vec[length_of_vector + normal_length * 2];
	for (int i = 0; i < normal_length*normal_length; i++)
	{
		vec_d[i] = static_cast<double>(vec[i]);
	}
	reduce_dimension(low_vec, vec_d);
	feature_of_row_col(row_col, vec_d);
	for (int i = 0; i < length_of_vector; i++)
	{
		feature_vec[i] = low_vec[i];
	}
	for (int j = 0; j < normal_length * 2; j++)
	{
		feature_vec[j + length_of_vector] = row_col[j];
	}
	return find_near_one(feature_vec, all_sample, type_of_distande);
}

double recognize::distance(double *vec1, double *vec2, int type_of_distande)//0 for Euler,1 for Cosine
{
	int n = length_of_vector;
	if (type_of_distande == EULER)
	{
		double dis_Euler = 0;
		for (int i = 0; i < n; i++)
		{
			dis_Euler += pow(vec1[i] - vec2[i], 2);
		}
		return pow(dis_Euler, 0.5);
	}
	if (type_of_distande == COSINE)
	{
		double dis_Cosine = 0;
		for (int i = 0; i < n; i++)
		{
			dis_Cosine += vec1[i] * vec2[i];
		}
		dis_Cosine = dis_Cosine / (module(vec1)*module(vec2));
		return dis_Cosine;
	}
	else
	{
		THROW_EXCEPTION("please input 1 or 0,0 for Eular distance and 1 for cosine distance");
		return -1;
	}
}

double recognize::module(double *vec1)
{
	int n = length_of_vector;
	double module_value = 0;
	for (int i = 0; i < n; i++)
	{
		module_value += pow(vec1[i], 2);
	}
	return pow(module_value, 0.5);
}

void recognize::reduce_dimension(double* output,double *vec)
{
	const int length_ = pow(length_of_vector, 0.5);
	for (int i = 0; i < length_; i++)
	{
		for (int j = 0; j < length_; j++)
		{
			output[i*length_ + j] = get_blockage(vec, i, j, int(normal_length/length_));
		}
	}
}

void recognize::feature_of_row_col(double* output, double *vec)
{
	//across and vertical feature
	//feature_vector is a list of 28 * 28, output is a list with 28 + 28
	int n = normal_length;
	double change_num = 0.0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (vec[n*i + j] != vec[n*i + j + 1])
			{
				change_num += 1.0;
			}
		}
		output[i] = change_num;
		change_num = 0.0;
	}
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n - 1; i++)
		{
			if (vec[n*i + j] != vec[n*(i + 1) + j])
			{
				change_num += 1.0;
			}
		}
		output[n+j] = change_num;
		change_num = 0.0;
	}
}

double recognize::get_blockage(double *vec, int i, int j, int n)
{
	double sum_ = 0;
	for (int col = 0; col < n; col++)
	{
		for (int row = 0; row < n; row++)
		{
			sum_ += vec[(n*i + col) * normal_length + n*j + row];
		}
	}
	return sum_;
}


int recognize::find_near_one(double *vec_test, std::vector<sample> vec_samples, int type_of_distande)
{
	if (vec_samples.size() == 0)
	{
		THROW_EXCEPTION("empty vectors");
		return -1;
	}
	if (type_of_distande == 0)
	{
		double min_dis = distance(vec_test, vec_samples[0].vec, type_of_distande);
		int min_id = 0;
		for (size_t i = 1; i < vec_samples.size(); i++)
		{
			double dis_save = distance(vec_test, vec_samples[i].vec, type_of_distande);
			if (dis_save < min_dis)
			{
				min_id = i;
				min_dis = dis_save;
			}
		}
		return vec_samples[min_id].index;
	}
	if (type_of_distande == 1)
	{
		double max_dis = distance(vec_test, vec_samples[0].vec, type_of_distande);
		int max_id = 0;
		double dis_save;
		for (size_t i = 1; i < vec_samples.size(); i++)
		{
			dis_save = distance(vec_test, vec_samples[i].vec, type_of_distande);
			if (dis_save > max_dis)
			{
				max_id = i;
				max_dis = dis_save;
			}
		}
		return vec_samples[max_id].index;
	}
	else
	{
		THROW_EXCEPTION("type_of_distande is wrong!");
		return -1;
	}
}
