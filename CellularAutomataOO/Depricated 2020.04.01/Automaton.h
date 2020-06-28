#pragma once

#include "Rule.h"

class Automaton
{
	private:
		Rule rule;
		static const int gridSize = 149; // Must be odd // Have used 199, but 149 by Mitchell
		static const int trials = 50; // May reduce runtime here
		int iterations = 160;
		static const int neighbors = 3;
		int grid[gridSize];
		// Might be problem with implicit converstion bool<->int
		int performance[trials];;
		double fitness;
		std::vector<int> cells = std::vector<int>(gridSize);
	public:
		Automaton();
		void calculateFitness();
		double getFitness() const { return fitness; }
		Automaton(const Automaton& mom, const Automaton& dad);
		void evolve();
		int classify(int majority) const;
		void initialize();
		void initializeNew(); // double topFitness
		void initializeNew(int index); // double topFitness
		void updateGrid();
		Automaton(const Automaton &other);
		Automaton operator=(Automaton rhs);
		Automaton(const Automaton mom, bool mutate);
		Automaton(const int arr[]);
		void visualize();
		void draw();
		void printRule();
		void operator()();
		void initializeCells();
		double getDensity() const;
		static bool sortByFitness(const std::shared_ptr<Automaton> &a1, const std::shared_ptr<Automaton> &a2);
};



