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

int* randomizeArray(int *arr, int length, int lower, int upper);

int countMajority(int arr[], int length);

int circularIndex(int index, int arrayLength);

void swapArrays(int lhs[], int rhs[], int length);

double randomProbability();

double randomRealWithLimits(double lower, double upper);

double randomFromDisjoint(double bottom, double lower, double upper, double top);

void testRandomWithLimits(int numCells, int repeats);

int testPoissonDist();

bool arrayContains(const int arr[], int length, int value);