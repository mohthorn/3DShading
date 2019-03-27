#pragma once
#ifndef __MEMORYCONTROL__
#define __MEMORYCONTROL__
#include <iostream>
using namespace std;

template <typename T>
void Aligned_2DArray(T** &twod_array, int wdt = 3, int hgt = 3,
	bool debug_mode = false)
{
	// To access data in 2D array, use A[y][x]
	// To access data in 1D array, use A[0][i]
	// Row Major Matrix
	T *a = new T[wdt * hgt];
	twod_array = new T *[hgt];

	for (int i = 0; i < hgt; ++i)
	{

		twod_array[i] = &a[i * wdt];

		// Print information for debug mode
		if (debug_mode)
		{
			std::cout << i << "th:\t" << &a[i * wdt] << std::endl;
			std::cout << "\t" << twod_array[i] << std::endl;
		}
	}
}
template <typename T>
void delete_2DArray(T** &twod_array, int wdt = 3, int hgt = 3,
	bool debug_mode = false)
{
	delete [] twod_array[0];
	for (int i = 0; i < hgt; i++)
	{
		if (debug_mode)
		{
			cout << twod_array[i][0] << endl;
		}
		twod_array[i] = nullptr;
	}
}
#endif // __MEMORYCONTROL__
