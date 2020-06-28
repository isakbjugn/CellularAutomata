#include "Functions.h"

std::stack<int> numberToDigits(int number)
{
	int length = int(floor(log10(number)));
	std::stack<int> digits;

	for (int i = 0; i < length; i++)
	{
		digits.push( int(floor(number / pow(10, length - i - 1))) );
		number -= int(digits.top() * pow(10, length - i - 1));
	}
	return digits;
}

int binaryToDec(int binary)
{
	std::stack<int> binaryStack = numberToDigits(binary);
	int dec = 0;
	for (unsigned i = 0; i < binaryStack.size(); i++)
	{
		dec = 2 * dec + binaryStack.top();
		binaryStack.pop();
	}
	return dec;
}

int binaryToDec(std::vector<int> binaryVec)
{
	int dec = 0;
	for (auto it = binaryVec.begin(); it != binaryVec.end(); it++)
	{
		dec = 2 * dec + *it;
	}
	return dec;
}

int binaryToDec(int binaryArr[], int length)
{
	int dec = 0;
	for (int i = 0; i < length; i++)
	{
		dec = 2 * dec + binaryArr[i];
	}
	return dec;
}

int randomWithLimits(int lower, int upper)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> uniform(lower, upper);
	int random_integer = uniform(generator);
	return random_integer;
}

double randomProbability()
{
	return randomRealWithLimits(0, 1);
}

double randomRealWithLimits(double lower, double upper)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_real_distribution<> uniform(lower, upper); // Error: was (0, 1)
	return uniform(generator);
}

int* randomizeArray(int *arr, int length, int lower, int upper)
{
	for (int i = 0; i < length; i++)
	{
		arr[i] = randomWithLimits(lower, upper);
	}
	return arr;
}

int countMajority(int arr[], int length)
{
	std::map<int, int> counts;

	for (int i = 0; i < length; i++)
	{
		counts[arr[i]]++;
	}

	int currentMax = 0;
	int maxIndex = 0;
	for (auto it = counts.begin(); it != counts.end(); it++)
	{
		if (it->second > currentMax)
		{
			currentMax = it->second;
			maxIndex = it->first;
		}
	}
	return maxIndex;
}

int circularIndex(int index, int arrayLength)
{
	while (index < 0)
	{
		index += arrayLength;
	}
	while (index >= arrayLength)
	{
		index -= arrayLength;
	}
	return index;
}

void swapArrays(int lhs[], int rhs[], int length)
{
	int temp;
	for (int i = 0; i < length; i++)
	{
		temp = lhs[i];
		lhs[i] = rhs[i];
		rhs[i] = temp;
	}
}

double randomFromDisjoint(double bottom, double lower, double upper, double top)
{
	if (bottom == lower)
	{
		return randomRealWithLimits(upper, top);
	}
	if (upper == top)
	{
		return randomRealWithLimits(bottom, lower);
	}
	double gap = upper - lower;
	double pick = randomRealWithLimits(bottom, top - gap);
	if (pick > lower)
	{
		pick += gap;
	}
	return pick;
}

void testRandomWithLimits(int numCells, int repeats)
{
	unsigned numOnes;
	unsigned numZeros;
	for (int j = 0; j < repeats; j++)
	{
		numOnes = 0;
		numZeros = 0;
		for (int i = 0; i < numCells; i++)
		{
			if (randomWithLimits(0, 1) == 1)
			{
				numOnes++;
			}
			else
			{
				numZeros++;
			}
		}
		std::cout << "Ones: " << numOnes << "\tZeros: " << numZeros << std::endl;
	}
}

int testPoissonDist()
{
	std::random_device device;
	std::mt19937 generator(device());
	std::poisson_distribution<int> distribution(230);

	return distribution(generator);
}

bool arrayContains(const int arr[], int length, int value)
{
	bool found = false;
	for (int i = 0; i < length; i++)
	{
		if (arr[i] == value)
		{
			return true;
		}
	}
	return false;
}