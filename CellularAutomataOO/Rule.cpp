#include "Rule.h"

Rule::Rule(int neighbors)
{
	this->neighbors = neighbors; // Originally forgot to implement this
	length = int(pow(2, 2 * neighbors + 1));
	rule = new int[length];
}

void Rule::mutate()
{
	int indeces[numMutations];
	randomIndeces(length, numMutations, indeces);

	for (int i = 0; i < numMutations; i++)
	{
		rule[indeces[i]] = 1 - rule[indeces[i]];
	}
}

void Rule::initialize()
{
	// Binomially distributed lambda
	randomizeArray(rule, length, 0, 1);

	// Uniformly distributed lambda
	int ones = randomWithLimits(0, length);
	std::vector<int> indeces = randomIndeces(length);

	for (int i = 0; i < ones; i++)
	{
		rule[indeces[i]] = 1;
	}
	for (int i = ones; i < length; i++)
	{
		rule[indeces[i]] = 0;
	}
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
	// When called during parents[0] = make_shared<Automaton>(*elite[indeces[0]]);,
	// it seems that *elite[0] has not an initialized rule.
	// This might have been avoided until now because one automaton has always been tested
	// before copying again
	for (int i = 0; i < length; i++)
	{
		rule[i] = other.rule[i];
		// While copying under parents[0] = make_shared<Automaton>(*elite[indeces[0]]);,
		// we observe the following behaviour:
		// other.rule[0]	->	0
		// this.rule[0]		->	3
		// other.rule[1]	->	1
		// this.rule[1]		->	128
		// Would this mean that we're currently not writing to this.rule?
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
