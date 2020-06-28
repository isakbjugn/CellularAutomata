#pragma once
#include <ostream>
#include <fstream>
#include "Rule.h"

class Automaton
{
	private:
		Rule rule;
		static const int gridSize = 149; // Must be odd // Have used 199, but 149 by Mitchell
		static const int trials = 100; // May reduce runtime here
		int iterations = 160;
		static const int neighbors = 3;
		int grid[gridSize];
		// Might be problem with implicit converstion bool<->int
		int performance[trials];;
		double fitness = -1;
	public:
		Automaton();
		void calculateFitness();
		double getFitness() const { return fitness; }
		Automaton(const Automaton& mom, const Automaton& dad);
		void evolve();
		int classify(int majority) const;
		void initializeBinomial();
		void initializeUniform(int index, double currentFitness = 0.0);
		void updateGrid();
		Automaton(const Automaton &other);
		Automaton operator=(Automaton rhs);
		Automaton(const Automaton mom, bool mutate);
		Automaton(const int arr[]);
		void visualize();
		void draw();
		void print();
		void printRule();
		void operator()(double currentFitness = 0.0);
		double getDensity() const;
		static bool sortByFitness(const std::shared_ptr<Automaton> &a1, const std::shared_ptr<Automaton> &a2);
};



