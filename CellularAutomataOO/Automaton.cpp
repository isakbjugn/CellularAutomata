#include "Automaton.h"

/*Constructors*/

Automaton::Automaton() : rule(neighbors)
{
	rule.initialize();
}

Automaton::Automaton(const Automaton& mom, const Automaton& dad) : rule(mom.rule, dad.rule, true)
{
	// rule = Rule(mom.rule, dad.rule, true); // This gives error?
}

Automaton::Automaton(const Automaton &other) : rule(other.rule)
{
	// Severe error was avoided by commenting out the next line, and rather
	// using a delegating constructor
	//rule = other.rule; 
	for (int i = 0; i < trials; i++)
	{
		performance[i] = other.performance[i];
	}
	fitness = other.fitness;
}

Automaton::Automaton(const Automaton mom, bool mutate) : rule(mom.rule, mutate)
{
	// Had to comment this out too
	//rule = Rule(mom.rule, mutate);
}

Automaton::Automaton(const int arr[]) : rule(arr, neighbors)
{
}

/*Operators*/

Automaton Automaton::operator=(Automaton rhs)
{
	// Is this right even when pointer belongs to member class,
	// not class itself?
	std::swap(rule, rhs.rule);
	return *this;
}

void Automaton::operator()(double currentFitness)
{
	// Declare variables
	int majority;
	int correct;

	// Test automaton on pattern for trials rounds
	for (int i = 0; i < trials; i++)
	{
		// Generate random initial configuration
		this->initializeUniform(i, currentFitness);

		// Determine nature of initial configuration (majority)
		majority = countMajority(grid, gridSize);

		// Let grid update for iterations updates
		for (int j = 0; j < iterations; j++)
		{
			updateGrid();
		}
		// Classify the end state after iterations updates
		correct = classify(majority);
		performance[i] = correct;
	}
	// Calculate fitness from performance array
	calculateFitness();
}

/*Functions*/

void Automaton::calculateFitness()
{
	int successes = 0;
	for (int i = 0; i < trials; i++)
	{
		successes += performance[i];
	}
	fitness = double(successes) /trials;
}

void Automaton::draw()
{
	for (int i = 0; i < gridSize; i++)
	{
		if (grid[i] == 0)
		{
			std::cout << "_";
		}
		else
		{
			std::cout << "#";
		}
	}
	std::cout << std::endl;
}

void Automaton::evolve()
{
	// Declare variables
	int majority;
	int correct;

	// Test automaton on pattern for trials rounds
	for (int i = 0; i < trials; i++)
	{
		// Generate random initial configuration
		this->initializeBinomial();

		// Determine nature of initial configuration (majority)
		majority = countMajority(grid, gridSize);

		// Let grid update for iterations updates
		for (int j = 0; j < iterations; j++)
		{
			updateGrid();
		}
		// Classify the end state after iterations updates
		correct = classify(majority);
		performance[i] = correct;
	}
	// Calculate fitness from performance array
	calculateFitness();
}

int Automaton::classify(int majority) const
{
	bool foundZero = arrayContains(grid, gridSize, 0);
	bool foundOne = arrayContains(grid, gridSize, 1);
	if (foundZero && foundOne)
	{
		return false;
	}
	if ((foundOne && majority) || (foundZero && !majority))
	{
		return true;
	}
	return false;
}

void Automaton::initializeBinomial()
{
	// Produces binomially distributed density
	randomizeArray(grid, gridSize, 0, 1);
	iterations = 230 + randomWithLimits(-10, 10);
}

void Automaton::initializeUniform(int index, double currentFitness)
{
	// Produces a uniformly distributed density

	// Increase difficulty
	double lowerBound = getLowerBound(currentFitness);
	double density = randomRealWithLimits(lowerBound, 0.5);
	int ones = int(floor(density*gridSize));
	// Exactly half of them with density > 1/2
	if (index % 2) // Check if index is odd or even
	{
		ones = gridSize - ones;
	}
	std::vector<int> indeces = randomIndeces(gridSize);

	for (int i = 0; i < ones; i++)
	{
		grid[indeces[i]] = 1;
	}
	for (int i = ones; i < gridSize; i++)
	{
		grid[indeces[i]] = 0;
	}

	iterations = 230 + randomWithLimits(-10, 10);
	return;
}

void Automaton::updateGrid()
{
	int newGrid[gridSize];
	int newIndex;
	int neighborhoodSize = 2 * neighbors + 1;
	int *neighborhood = new int[neighborhoodSize];
	for (int i = 0; i < gridSize; i++) {
		for (int j = -neighbors; j < neighbors + 1; j++)
		{
			newIndex = circularIndex(i+j, gridSize);
			
			neighborhood[j + neighbors] = grid[newIndex];
		}
		newGrid[i] = rule.getRule(neighborhood, neighborhoodSize);

	}
	//swapArrays(grid, newGrid, gridSize); // This is slower
	for (int i = 0; i < gridSize; i++)
	{
		grid[i] = newGrid[i];
	}
	delete neighborhood;
	neighborhood = nullptr;
}

void Automaton::visualize()
{
	// Declare variables
	int majority;
	double density;

	// Generate random initial pattern
	//this->initializeBinomial();
	this->initializeUniform(randomBit(), 0.98);

	// Classify and print initial pattern
	majority = countMajority(grid, gridSize);
	density = getDensity();
	std::cout << "\nInitial majority: " << ((majority) ? "#" : "_")
		<< ", density: " << density << "\n";
	draw();

	// Let grid update for iterations updates, draw as you go
	for (int j = 0; j < iterations; j++)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(25));
		updateGrid();

		// Activate the following line to reduce print speed
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		draw();
	}

	bool correct = classify(majority);
	// performance[0] = correct;

	std::cout << "Trial " << ((correct) ? "" : "un") << "successful\n";
}

void Automaton::print()
{
	// Declare variables
	int majority;
	double density;

	// Generate random initial pattern
	//this->initializeBinomial();
	this->initializeUniform(randomBit(), 0.98);

	// Classify and print initial pattern
	majority = countMajority(grid, gridSize);
	density = getDensity();
	std::cout << "\nInitial majority: " << ((majority) ? "#" : "_")
		<< ", density: " << density << "\n";
	draw();

	// Open file
	std::ofstream myfile;
	myfile.open("ca-grid.dat", std::ios::trunc);

	// Let grid update for iterations updates, draw as you go
	for (int j = 0; j < iterations; j++)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(25));
		updateGrid();

		// Activate the following line to reduce print speed
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		draw();

		// Draw to file
		for (int i = 0; i < gridSize; i++)
		{
			myfile << grid[i] << " ";
		}
		myfile << "\n";
	}

	// Close file
	myfile.close();

	bool correct = classify(majority);
	// performance[0] = correct;

	std::cout << "Trial " << ((correct) ? "" : "un") << "successful\n";

	// Write details to file
	myfile.open("ca-performance.dat", std::ios::trunc);
	myfile << majority << " " << density << " " << correct << "\n";
	myfile.close();
}

void Automaton::printRule() {
	std::cout << "\nAutomaton rule:\n";
	for (int i = 0; i < int(pow(2, 2 * neighbors + 1)); i++)
	{
		std::cout << rule.getRule(i) << " ";
	}
	std::cout << std::endl;
}

double Automaton::getDensity() const
{
	int ones = 0;
	for (int i = 0; i < gridSize; i++)
	{
		ones += (grid[i] == 1);
	}
	return double(ones) / gridSize;
}

bool Automaton::sortByFitness(const std::shared_ptr<Automaton> &a1, const std::shared_ptr<Automaton> &a2)
{
	return a1->fitness > a2->fitness;
}