#include "Rule.h"

Rule::Rule(int neighbors)
{
	this->neighbors = neighbors; // Originally forgot to implement this
	length = int(pow(2, 2 * neighbors + 1));
	rule = new int[length];
}

void Rule::mutate()
{
	for (int i = 0; i < length; i++)
	{
		if (double(randomWithLimits(0, 100)) / 100 < mutateChance)
		{
			rule[i] = 1 - rule[i];
		}
	}
}

void Rule::initialize()
{
	// Possible, depending on how dynamical arrays are treated
	rule = randomizeArray(rule, length, 0, 1);

	// Always possible
	/*for (int i = 0; i < length; i++)
	{
		rule[i] = randomWithLimits(0, 1);
	}*/
}

Rule::Rule(const Rule &parent, bool mutate) : Rule(parent)
{
	if (mutate)
	{
		this->mutate();
	}
}

Rule::Rule(const Rule &mom, const Rule &dad, bool mutate) : Rule(mom.neighbors)
{
	int splice = randomWithLimits(0, length - 1);
	for (int i = 0; i < splice; i++)
	{
		rule[i] = mom.rule[i];
	}
	for (int i = splice; i < length; i++)
	{
		rule[i] = dad.rule[i];
	}

	if (mutate)
	{
		this->mutate();
	}
}

Rule::Rule(const Rule& other) : Rule(other.neighbors)
{
	for (int i = 0; i < length; i++)
	{
		rule[i] = other.rule[i];
	}
}

Rule Rule::operator=(Rule rhs)
{
	// Thorough debugging indicates something is off here
	std::swap(rule, rhs.rule);
	return *this;
}

Rule::~Rule()
{
	delete rule;
	rule = nullptr;
}

int Rule::getRule(int arr[], int length) const
{
	/*std::cout << "neighborhood:\t";
	for (int i = 0; i < length; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;*/
	int index = binaryToDec(arr, length);
	return rule[index];
}

Rule::Rule(const int arr[], unsigned radius) : Rule(radius)
{
	int length = int( pow(2, 2 * radius + 1) );
	for (int i = 0; i < length; i++)
	{
		rule[i] = arr[i];
	}
}
