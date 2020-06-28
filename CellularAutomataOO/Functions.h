#pragma once
#include <math.h>
#include <iterator>
#include <vector>
#include <stack>
#include <random>
#include <map>
#include <iostream>
#include <thread>
#include <chrono>
#include <thread>
#include <algorithm>

std::stack<int> numberToDigits(int number);

int binaryToDec(int binary);

int binaryToDec(std::vector<int> binaryVec);

int binaryToDec(int binaryArr[], int length);

int randomWithLimits(int lower, int upper);

void randomizeArray(int *arr, int length, int lower, int upper); // Could be called binomialArray

int countMajority(int arr[], int length);

int circularIndex(int index, int arrayLength);

void swapArrays(int lhs[], int rhs[], int length);

double randomProbability();

double randomRealWithLimits(double lower, double upper);

double randomFromDisjoint(double bottom, double lower, double upper, double top);

void testRandomWithLimits(int numCells, int repeats);

int testPoissonDist();

bool arrayContains(const int arr[], int length, int value);

void randomIndeces(int values, int number, int output[]);

std::vector<int> randomIndeces(int values);

void testRandomIndeces();

bool randomBit();

double getLowerBound(double fitness);

/*Template functions*/
template<typename T>
void saveVector(const char* filename, const std::vector<T>& vec);

/*Implementation of template functions*/

template<typename T>
void saveVector(const char* filename, const std::vector<T>& vec)
{
	std::ofstream myfile;
	myfile.open(filename, std::ios::trunc);

	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		myfile << *it << " ";
	}
	myfile << "\n";
	myfile.close();
}