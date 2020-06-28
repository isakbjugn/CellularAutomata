#pragma once
#include "Functions.h"

class Rule
{
	private:
		int neighbors;
		int length;
		int* rule;
		static constexpr double mutateChance = 0.008;
	public:
		Rule() { rule = nullptr; }
		Rule(int neighbors);
		Rule(const Rule &parent, bool mutate);
		Rule(const Rule &mom, const Rule &dad, bool mutate = true);
		void mutate();
		void initialize();
		Rule(const Rule &other);
		Rule operator=(Rule);
		~Rule();
		int getRule(int index) const { return rule[index]; }
		int getRule(int arr[], int length) const;
		Rule(const int arr[], unsigned radius);
};