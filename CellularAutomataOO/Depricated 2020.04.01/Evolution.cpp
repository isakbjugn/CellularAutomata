#include "Evolution.h"

int evolution(int population, int generations)
{
	// Declare variables
	vector<shared_ptr<Automaton>> generation;
	vector<shared_ptr<Automaton>> newGeneration;
	vector<shared_ptr<Automaton>> parents;
	shared_ptr<Automaton> parent;
	shared_ptr<Automaton> winner;
	
	// Initialise start population
	for (int i = 0; i < population; i++)
	{
		// Initialize automaton with random rule
		//generation.push_back( shared_ptr<Automaton>(new Automaton()) ); // Is this enough?
		generation.push_back( make_shared<Automaton>( )); // Is this enough?
	}

	// Loop which iterates through generations
	for (int i = 0; i < generations; i++)
	{
		// Print status
		std::cout << "Generation " << i;

		// Iterate through current population
		for (auto it = generation.begin(); it != generation.end(); it++)
		{
			// Capacity to perform multiple trials is incorporated in Automaton
			(*it)->evolve();
			//(*it)->visualize();
		}

		// Print status
		std::cout << "\t-selection commenced-\t";

		// Initialise new population
		for (int j = 0; j < population; j++)
		{
			//std::cout << "Fint 1\n";
			if (randomProbability() < 0.5)
			{
				// Choose one individual which survives
				parent = selectIndividual(generation);
				newGeneration.push_back( make_shared<Automaton>(*parent, true) );
				parent.reset();
			}
			else
			{
				// Choose two individuals to breed
				parents = selectParents(generation);
				//mom.reset(new Automaton(*parents[0]));
				//dad.reset(new Automaton(*parents[1]));

				// Problem seems to be that the newGeneration are initialized with crazy
				// values for everything but the grid. It updates as normal, but is not
				// capable of giving rise to new Automatons, which is why it crashed at
				// the next line

				newGeneration.push_back(make_shared<Automaton>(*parents[0], *parents[1]));
				
				parents.clear();
			}
		}
		// Possible extension: Store highest performing automaton from each generation
		// Alternative: Print highest fitness
		std::cout << "Highest fitness: " << findFittest(generation)->getFitness() << "\n";

		// Replace old population with new population
		generation.clear();
		generation = newGeneration;
		newGeneration.clear();

	}
	// Select highest performing automaton from generation
	winner = findFittest(generation);
	// Assess strategy by visualization
	//winner->evolveSimple();
	//winner->printRule();

	return 0;
}

int evolutionParallel(int population, int generations)
{
	// Declare variables
	vector<shared_ptr<Automaton>> generation;
	vector<shared_ptr<Automaton>> newGeneration;
	vector<shared_ptr<Automaton>> parents;
	shared_ptr<Automaton> parent;
	shared_ptr<Automaton> winner;
	vector<std::thread> threads(population);

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
			threads[i] = std::thread(std::ref(*individual));
			// Must use std::ref(), else the thread makes a copy and does not alter original object
		}

		for (int i = 0; i < population; i++)
		{
			threads[i].join();
		}

		// End evolution if last generation
		if (i == (generations - 1))
		{
			break;
		}

		// Print status
		std::cout << "\tHighest fitness: " << findFittest(generation)->getFitness();
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

	return 0;
}

shared_ptr<Automaton> selectIndividual(vector<shared_ptr<Automaton>> &generation)
{
	// New, pointer-based overload

	double baseFitness = 0.001;
	double accFitness = 0;
	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += (*it)->getFitness() + baseFitness;
	}

	double pick = randomRealWithLimits(0, accFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += (*it)->getFitness() + baseFitness;
		if (accFitness > pick)
		{
			return *it;
		}
	}
	std::cout << "No fit individuals found!\n";
	return nullptr;
}

vector<shared_ptr<Automaton>> selectDescendants(vector<shared_ptr<Automaton>> &generation, double gap)
{
	// Newest implementation
	vector<shared_ptr<Automaton>> elite;
	vector<shared_ptr<Automaton>> newGeneration;
	vector<shared_ptr<Automaton>> parents;

	int numElite = int(round(generation.size() * gap));
	std::sort(generation.begin(), generation.end(), Automaton::sortByFitness);

	for (int i = 0; i < numElite; i++)
	{
		elite.push_back(make_shared<Automaton>(*generation[i]));
		newGeneration.push_back(make_shared<Automaton>(*generation[i]));
	}
	for (int i = numElite; i < generation.size(); i++)
	{
		parents = selectParentsNew(elite);
		newGeneration.push_back(make_shared<Automaton>(*parents[0], *parents[1]));
		parents.clear();
	}
	return newGeneration;
}

Automaton selectIndividual(vector<Automaton> &generation)
{
	// Original overload of selectIndividual
	double baseFitness = 0.001;
	double accFitness = 0;
	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += it->getFitness() + baseFitness;
	}

	double pick = randomRealWithLimits(0, accFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += it->getFitness() + baseFitness;
		if (accFitness > pick)
		{
			return Automaton(*it);
		}
	}
	std::cout << "Kvekk! Ingen gode individer ble funnet!\n";
	std::cout << "Første individ i generasjonen ble returnert.\n";
	return Automaton(generation[0]);
}

vector<shared_ptr<Automaton>> selectParentsNew(vector<shared_ptr<Automaton>> &elite)
{
	vector<int> indeces = vector<int>(elite.size());
	for (int i = 0; i < elite.size(); i++)
	{
		indeces[i] = i;
	}
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(indeces.begin(), indeces.end(), g);

	vector<shared_ptr<Automaton>> parents;
	parents[0] = make_shared<Automaton>(*elite[0]);
	parents[1] = make_shared<Automaton>(*elite[1]);
	return parents;
}

vector<shared_ptr<Automaton>> selectParents(vector<shared_ptr<Automaton>> &generation)
{
	// New, pointer-based overload

	vector<shared_ptr<Automaton>> parents;
	double baseFitness = 0.001;
	double totalFitness = 0;
	double accFitness = 0;
	double lowerBin = 0;
	double upperBin = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		totalFitness += (*it)->getFitness() + baseFitness;
	}

	double pick = randomRealWithLimits(0, totalFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += (*it)->getFitness() + baseFitness;
		if (accFitness > pick)
		{
			parents.push_back(*it);
			lowerBin = accFitness - (*it)->getFitness() - baseFitness;
			upperBin = accFitness;
			break;
		}
	}

	pick = randomFromDisjoint(0, lowerBin, upperBin, totalFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += (*it)->getFitness() + baseFitness;
		if (accFitness > pick)
		{

			parents.push_back(*it);
			break;
		}
	}
	return parents;
}

vector<Automaton> selectParents(vector<Automaton> &generation)
{
	// Original overload of selectParents
	std::vector<Automaton> parents;
	double baseFitness = 0.001;
	double totalFitness = 0;
	double accFitness = 0;
	double lowerBin = 0;
	double upperBin = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		totalFitness += it->getFitness() + baseFitness;
	}

	double pick = randomRealWithLimits(0, totalFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += it->getFitness() + baseFitness;
		if (accFitness > pick)
		{
			parents.push_back(Automaton(*it));
			lowerBin = accFitness - it->getFitness() - baseFitness;
			upperBin = accFitness;
			break;
		}
	}

	pick = randomFromDisjoint(0, lowerBin, upperBin, totalFitness);
	accFitness = 0;

	for (auto it = generation.begin(); it < generation.end(); it++)
	{
		accFitness += it->getFitness() + baseFitness;
		if (accFitness > pick)
		{
			parents.push_back(Automaton(*it));
			break;
		}
	}
	return parents;
}

shared_ptr<Automaton> findFittest(vector<shared_ptr<Automaton>> &generation)
{
	// New, pointer-based overload

	shared_ptr<Automaton> winner;
	double maxFitness = -1; // Must allow individuals with fitness 0.0
	double fitness;
	for (auto it = generation.begin(); it != generation.end(); it++)
	{
		fitness = (*it)->getFitness();
		if (fitness > maxFitness)
		{
			maxFitness = fitness;
			winner = *it;
		}
	}
	return winner;
}

Automaton findFittest(vector<Automaton> &generation)
{
	// Original overload of findFittest
	Automaton winner;
	double maxFitness = 0;
	double fitness;
	for (auto it = generation.begin(); it != generation.end(); it++)
	{
		fitness = it->getFitness();
		if (fitness > maxFitness)
		{
			maxFitness = fitness;
			winner = *it;
		}
	}
	return winner;
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
	GKL.visualize();
}

void testArbitraryRule()
{
	int rule[128] = { 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1 };
	Automaton Noah(rule);
	Noah.visualize();
}

void testDensity()
{
	Automaton Adam;
	for (int i = 0; i < 100; i++)
	{
		Adam.initializeNew();
		std::cout << "Density: " << Adam.getDensity() << "\n";
	}
}