#include "Evolution.h"

int evolution(int population, int generations)
{
	// Declare variables
	vector<shared_ptr<Automaton>> generation;
	vector<double> fitnesses;
	fitnesses.reserve(generations);
	generation.reserve(population);
	shared_ptr<Automaton> winner;
	vector<std::thread> threads(population);
	double currentFitness = 0;

	// Initialise start population
	for (int i = 0; i < population; i++)
	{
		// Initialize automaton with random rule
		generation.push_back(make_shared<Automaton>()); // Is this enough?
	}

	// Loop which iterates through generations
	for (int i = 0; i < generations; i++)
	{
		// Print status
		std::cout << "Generation " << i;

		// Iterate through current population
		for (int i = 0; i < population; i++)
		{
			shared_ptr<Automaton> individual = generation[i];
			threads[i] = std::thread(std::ref(*individual), currentFitness);
			// Must use std::ref(), else the thread makes a copy and does not alter original object
		}

		for (int i = 0; i < population; i++)
		{
			threads[i].join();
		}
		// Print status
		currentFitness = findFittest(generation)->getFitness();
		fitnesses.push_back(currentFitness);
		std::cout << "\tHighest fitness: " << currentFitness;

		// End evolution if last generation
		if (i == (generations - 1))
		{
			break;
		}
		
		std::cout << "\t-selection commenced-\n";

		// Initialise new population
		generation = selectDescendants(generation, 0.2);

		// Possible extension: Store highest performing automaton from each generation
	}
	// Select highest performing automaton from generation
	winner = findFittest(generation);

	// Assess strategy by visualization
	winner->visualize();
	winner->printRule();
	std::cout << "Fitness: " << winner->getFitness() << "\n";
	
	// Produce fitness statistics
	std::cout << "Fitnesses: ";
	for (auto it = fitnesses.begin(); it != fitnesses.end(); it++)
	{
		std::cout << *it << ", ";
	}
	std::cout << "\n";

	return 0;
}

vector<shared_ptr<Automaton>> selectDescendants(vector<shared_ptr<Automaton>> &generation, double gap)
{
	// Newest implementation
	vector<shared_ptr<Automaton>> elite;
	vector<shared_ptr<Automaton>> newGeneration;
	newGeneration.reserve(generation.size());
	vector<shared_ptr<Automaton>> parents;
	parents.reserve(2);

	int numElite = int(round(generation.size() * gap));
	elite.reserve(numElite);

	std::sort(generation.begin(), generation.end(), Automaton::sortByFitness);

	for (int i = 0; i < numElite; i++)
	{
		elite.push_back(make_shared<Automaton>(*generation[i]));
		newGeneration.push_back(make_shared<Automaton>(*generation[i]));
	}
	for (unsigned i = numElite; i < generation.size(); i++)
	{
		parents = selectParents(elite);
		newGeneration.push_back(make_shared<Automaton>(*parents[0], *parents[1]));
		parents.clear();
	}
	return newGeneration;
}

vector<shared_ptr<Automaton>> selectParents(vector<shared_ptr<Automaton>> &elite)
{
	vector<int> indeces = randomIndeces(elite.size());

	vector<shared_ptr<Automaton>> parents;
	parents.reserve(2);

	// Yes, you really have to use push_back, not [i]
	parents.push_back( make_shared<Automaton>(*elite[indeces[0]]) );
	parents.push_back( make_shared<Automaton>(*elite[indeces[1]]) );
	return parents;
}

shared_ptr<Automaton> findFittest(vector<shared_ptr<Automaton>> &generation)
{
	std::sort(generation.begin(), generation.end(), Automaton::sortByFitness);
	return generation[0];
}

void testAutomatonConstructor()
{
	Automaton Adam;
	Adam.evolve();
	std::cout << "Automaton fitness: " << Adam.getFitness() << std::endl;

	Automaton Eve(Adam);
	Eve.evolve();
	std::cout << "Automaton fitness: " << Eve.getFitness() << std::endl;

	Automaton Datsa = Adam;
	Datsa.evolve();
	std::cout << "Automaton fitness: " << Datsa.getFitness() << std::endl;
}

void testAutomatonVector()
{
	Automaton Adam;
	Automaton Eve;

	std::vector<Automaton> brothers;
	Automaton Cain(Adam, Eve);
	Automaton Abel(Adam, Eve);
	brothers.push_back(Cain);
	brothers.push_back(Abel);
	brothers[0].evolve();
	std::cout << "Automaton fitness: " << brothers[0].getFitness() << std::endl;
}

void testAutomatonPointerVector()
{
	Automaton Adam;
	Automaton Eve;

	std::vector<Automaton*> brothers;
	Automaton *Cain = new Automaton(Adam, Eve);
	Automaton *Abel = new Automaton(Adam, Eve);
	brothers.push_back(Cain);
	brothers.push_back(Abel);
	brothers[0]->evolve();
	brothers[1]->evolve();
	std::cout << "Automaton fitness: " << brothers[0]->getFitness() << std::endl;
	std::cout << "Automaton fitness: " << brothers[1]->getFitness() << std::endl;
}

void testAutomatonSmartPointerVector()
{
	Automaton Adam;
	Automaton Eve;

	// Must employ shared_ptr, not unique_ptr (both are smart pointers)
	// There can only be one copy of unique_ptr, and these thus cannot be used
	// in a vector which will be copied (e.g. when using push_back() without having
	// reserved enough memory for the vector.

	std::vector<std::shared_ptr<Automaton>> brothers;
	std::shared_ptr<Automaton> Cain(new Automaton(Adam, Eve));
	std::shared_ptr<Automaton> Abel(new Automaton(Adam, Eve));
	brothers.push_back(Cain);
	brothers.push_back(Abel);
	brothers[0]->evolve();	
	brothers[1]->evolve();
	std::cout << "Automaton fitness: " << brothers[0]->getFitness() << std::endl;
	std::cout << "Automaton fitness: " << brothers[1]->getFitness() << std::endl;
	brothers.push_back(shared_ptr<Automaton>(new Automaton()));
	brothers[2]->evolve();
	std::cout << "Automaton fitness: " << brothers[2]->getFitness() << std::endl;
}

void testAutomatonBreeding()
{
	Automaton Adam;
	Automaton Eve;
	Automaton Datsa(Adam, Eve);
	shared_ptr<Automaton> Cain = make_shared<Automaton>(Automaton(Adam, Eve));
	Cain->visualize();
	shared_ptr<Automaton> Abel = make_shared<Automaton>(Automaton(*Cain, Eve));
	Abel->visualize();
}

void testRulePrint()
{
	Automaton Adam;
	Adam.visualize();
	Adam.printRule();
}

void testGKLRule()
{
	int GKLRule[128] = { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
		0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1,
		0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1 };
	Automaton GKL(GKLRule);
	//GKL.visualize();
	GKL.print();
}

void testArbitraryRule()
{
	// Run 2020-04-01-12-00
	//int rule[128] = { 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 };
	
	//int rule[128] = { 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1 };
	
	// Run 2020-04-01-16-52
	//int rule[128] = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1 };
	
	// Run 2020-04-02-09-21
	//int rule[128] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	// Run 2020-04-02-12-39
	int rule[128] = { 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 };

	Automaton Noah(rule);
	//Noah.visualize();
	Noah.print();
}

void testDensity()
{
	Automaton Adam;
	for (int i = 0; i < 100; i++)
	{
		Adam.initializeUniform(randomBit());
		std::cout << "Density: " << Adam.getDensity() << "\n";
	}
}

void testHard()
{
	int rule[128] = { 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 };
	Automaton Moses(rule);
	double fitness = 0.0;
	int epiTrials = 100;
	std::cout << "Trial: ";
	for (int i = 0; i < epiTrials; i++)
	{
		std::cout << i << " ";
		Moses();
		fitness += Moses.getFitness();
	}
	std::cout << "\nTotal fitness: " << fitness/epiTrials << "\n";
	// Run 2020-04-01-12-00: Total fitness was 0.9266
}