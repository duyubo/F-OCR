#pragma once
#include<vector>
#include<map>
#define LOAD_SAMPLE_FROM_FILE
#define length_of_vector 49
#define normal_length 28

struct sample
{
	int index;
	double vec[length_of_vector + 2 * normal_length];
};

typedef unsigned char byte;

class recognize
{
public:
#ifdef _DEBUG
	static void load_sample_from_txtfile(char *sampleFileName);
	static void create_sample_file(char *dest, char *s);
#endif
#ifdef LOAD_SAMPLE_FROM_FILE
	static void load_sample(char *sampleFileName);
#endif // DEBUG
	static int GetIndex(byte *vec, int type_of_distande);
private:
	static void reduce_dimension(double* output, double *vec);
	static void feature_of_row_col(double* output, double *vec);
	static double distance(double *vec1,double *vec2,int type_of_distande);//0 for Euler,1 for Cosine
	static double module(double *vec1);
	static double get_blockage(double *vec, int i, int j, int n);
	static int find_near_one(double *vec_test, std::vector<sample> vec_samples,int type_of_distande);
private:
	static std::vector<sample> all_sample;
};


#define EULER 0
#define COSINE 1
