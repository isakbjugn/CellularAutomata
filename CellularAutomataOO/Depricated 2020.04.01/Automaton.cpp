#include "Automaton.h"

Automaton::Automaton() : rule(neighbors)
{
	rule.initialize();
	initializeCells();
}

void Automaton::initializeCells()
{
	// Needed for IC with uniform density distribution
	for (int i = 0; i < gridSize; i++)
	{
		cells[i] = i;
	}
}

Automaton::Automaton(const Automaton& mom, const Automaton& dad) : rule(mom.rule, dad.rule, true)
{
	// rule = Rule(mom.rule, dad.rule, true); // This gives error?
	//grid = std::vector<bool>(200);

	initializeCells();
}

Automaton::Automaton(const Automaton &other) : rule(other.rule)
{
	// Severe error was avoided by commenting out the next line, and rather
	// using a delegating constructor
	//rule = other.rule; 

	// Why would we copy grid? Is always overwritten
	/*for (int i = 0; i < gridSize; i++)
	{
		grid[i] = other.grid[i];
	}*/
	for (int i = 0; i < trials; i++)
	{
		performance[i] = other.performance[i];
	}
	fitness = other.fitness;

	initializeCells();
}

Automaton Automaton::operator=(Automaton rhs)
{
	// Is this right even when pointer belongs to member class,
	// not class itself?
	std::swap(rule, rhs.rule);
	return *this;
}

Automaton::Automaton(const Automaton mom, bool mutate) : rule(mom.rule, mutate)
{
	// Had to comment this out too
	//rule = Rule(mom.rule, mutate);
	initializeCells();
}

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
		// Keep us updated
		//std::cout << "Trial " << i << " commenced\n";

		// Generate random initial configuration
		this->initialize();

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

void Automaton::initialize()
{
	// Corresponds to a density binomially distributed
	for (int i = 0; i < gridSize; i++)
	{
		grid[i] = randomWithLimits(0, 1);
	}
	// New since 28.03.2020 19:52
	//std::poisson_distribution<int>();
}

void Automaton::initializeNew() // double topFitness
{
	// Produces a uniformly distributed density
	double density = randomProbability();
	int ones = int(round(density*gridSize));
	
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(cells.begin(), cells.end(), g);
	
	for (int i = 0; i < ones; i++)
	{
		grid[cells[i]] = 1;
	}
	for (int i = ones; i < gridSize; i++)
	{
		grid[cells[i]] = 0;
	}

	// Extension: Restrict density towards 1/2 for higher topFitness

	// Extension: Draw #iterations from Poission distribution
	iterations = 230 + randomWithLimits(-10, 10);

	return;
}

void Automaton::initializeNew(int index) // double topFitness
{
	// Produces a uniformly distributed density
	// Exactly half of them with density > 1/2

	double density;
	// Check if index is odd or even
	if (index % 2)
	{
		density = randomRealWithLimits(0, 0.5);
	}
	else
	{
		density = randomRealWithLimits(0.5, 1);
	}
	int ones = int(round(density*gridSize));

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(cells.begin(), cells.end(), g);

	for (int i = 0; i < ones; i++)
	{
		grid[cells[i]] = 1;
	}
	for (int i = ones; i < gridSize; i++)
	{
		grid[cells[i]] = 0;
	}

	// Extension: Restrict density towards 1/2 for higher topFitness

	// Extension: Draw #iterations from Poission distribution
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
		//std::cout << "Cell " << i << ", ";
		for (int j = -neighbors; j < neighbors + 1; j++)
		{
			newIndex = circularIndex(i+j, gridSize); // i+j, essential!?
			
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
	this->initialize();

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
		draw();
	}

	bool correct = classify(majority);
	// performance[0] = correct;

	std::cout << "Trial " << ((correct) ? "" : "un") << "successful\n";
}

void Automaton::printRule() {
	std::cout << "\nAutomaton rule:\n";
	for (int i = 0; i < int(pow(2, 2 * neighbors + 1)); i++)
	{
		std::cout << rule.getRule(i) << " ";
	}
	std::cout << std::endl;
}

Automaton::Automaton(const int arr[]) : rule(arr, neighbors)
{
	initializeCells();
}

void Automaton::operator()()
{
	// Declare variables
	int majority;
	int correct;

	// Test automaton on pattern for trials rounds
	for (int i = 0; i < trials; i++)
	{
		// Generate random initial configuration
		this->initializeNew(i);

		// Determine nature of initial configuration (majority)
		majority = countMajority(grid, gridSize);

		// Let grid update for iterations updates
		for (int j = 0; j < iterations; j++)
		{
			updateGrid();
		}

		// Classify the end state after iterations updates
		correct = classify(majority);
		/*if (correct)
		{
			std::cout << "Kvekk!\n";
		}*/
		performance[i] = correct;
	}

	// Calculate fitness from performance array
	calculateFitness();
	//std::cout << "Fitness: " << getFitness() << ", ID=" << std::this_thread::get_id() << "\n";
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