#include <iostream>
#include <Windows.h>
#include <xmmintrin.h>
#include <iomanip>
using namespace std;

void fill_array(float* a, float* b, int size);
void dotProduct_reg(float* a, float* b, float* result, int size);
void dotProduct_opt(float* a, float* b, float* temp, float* result, int size);

int main()
{
	const int sizes = 18;
	int size[sizes] = {60, 120, 600, 1200, 6000, 12000, 60000, 120000, 600000, 1200000, 6000000, 12000000, 60000000, 72000000, 84000000, 96000000, 108000000, 120000000};
	float *arr1; 
	float *arr2;
	float *result = new float();
	float *temp = new float();
	__int64 ctr1, ctr2, freq;
	
	for(int i = 0; i < sizes; i++)
	{
		// initialize
		ctr1 = 0, ctr2 = 0, freq = 0;
		*result = 0;
		const int ARRAY_SIZE = size[i];
		arr1 = new float[ARRAY_SIZE];
		arr2 = new float[ARRAY_SIZE];

		// fill arrays
		fill_array(arr1, arr2, ARRAY_SIZE);

		// UNOPTIMIZED INNER PRODUCT
		cout << "----------" << endl;
		if(QueryPerformanceCounter((LARGE_INTEGER *)&ctr1) != 0)
		{
			dotProduct_reg(arr1, arr2, result, ARRAY_SIZE);
			// cout << "(" << i << ")" << "     " << "dotProduct_reg() => *result = " << *result << endl; // TO CHECK RESULT
			QueryPerformanceCounter((LARGE_INTEGER *)&ctr2);
			QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
			cout << "(" << i << ")" << " size = " << setw(8) << size[i] << ", " << "Time (microseconds) for UNOPTIMIZED = " << (((double)(ctr2 - ctr1))/freq)*1000000 << endl;	// in microseconds
		}	

		ctr1 = 0; ctr2 = 0; freq = 0;
		*result = 0; *temp = 0;

		// OPTIMIZED INNER PRODUCT
		if(QueryPerformanceCounter((LARGE_INTEGER *)&ctr1) != 0)
		{
			dotProduct_opt(arr1, arr2, temp, result, ARRAY_SIZE);
			// cout << "(" << i << ")" << "     " << "dotProduct_opt() => *result = " << *result << endl; // TO CHECK RESULT
			QueryPerformanceCounter((LARGE_INTEGER *)&ctr2);
			QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
			cout <<  "(" << i << ")" << " size = " << setw(8) << size[i] << ", " << "Time (microseconds) for OPTIMIZED   = " << (((double)(ctr2 - ctr1))/freq)*1000000 << endl;	// in microseconds
		}

		delete [] arr1;
		delete [] arr2;
	}
	cout << "----------" << endl;
	cin.get();
	return 0;
}

void fill_array(float* a, float* b, int size)
{
	float *p; float *q;
	p = &a[0]; q = &b[0];

	// fill with 1s
	for(; p < &a[size]; p = p + 1, q = q + 1)
	{
		*p = 1; 
		*q = 1;
	}
}

void dotProduct_opt(float *a, float *b, float *temp, float* result, int size)
{		
	__m128 XMM0 = _mm_setzero_ps();
	__m128 XMM1 = _mm_setzero_ps();
	__m128 XMM2 = _mm_setzero_ps();
	__m128 XMM3 = _mm_setzero_ps();
	__m128 XMM4 = _mm_setzero_ps();
	__m128 XMM5 = _mm_setzero_ps();
	__m128 XMM6 = _mm_setzero_ps();
	__m128 XMM7 = _mm_setzero_ps();
	__m128 XMM8 = _mm_setzero_ps();
	__m128 XMMTotal = _mm_setzero_ps();

	for(int i = 0; (i+12) <= size; i += 12)
	{
		XMM0 = _mm_loadu_ps(a + i);		
		XMM1 = _mm_loadu_ps(a + i + 4);		
		XMM2 = _mm_loadu_ps(a + i + 8);	
		XMM3 = _mm_loadu_ps(b + i);
		XMM4 = _mm_loadu_ps(b + i + 4);
		XMM5 = _mm_loadu_ps(b + i + 8);

		XMM6 = _mm_mul_ps(XMM0, XMM3);
		XMM7 = _mm_mul_ps(XMM1, XMM4);
		XMM8 = _mm_mul_ps(XMM2, XMM5);

		XMM0 = _mm_add_ps(XMM6, XMM7);
		XMMTotal = _mm_add_ps(XMM0, XMM8);
		_mm_storeu_ps(temp, XMMTotal);
		*result += (*temp + *(temp+1) + *(temp+2) + *(temp+3));
	}
}


void dotProduct_reg(float *a, float *b, float* result, int size)
{
	float *p; float *q;
	p = &a[0]; q = &b[0];
	for(; p < &a[size]; p = p + 1, q = q + 1)
		*result += *p * *q;
}
