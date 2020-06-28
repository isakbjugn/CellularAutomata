#pragma once
#include "Automaton.h"
#include <queue>	

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::move;

int evolution(int population, int generations);

int evolutionParallel(int population, int generations);

Automaton selectIndividual(vector<Automaton> &generation);
shared_ptr<Automaton> selectIndividual(vector<shared_ptr<Automaton>> &generation);
vector<shared_ptr<Automaton>> selectDescendants(vector<shared_ptr<Automaton>> &generation, double gap);

vector<Automaton> selectParents(vector<Automaton> &generation);
vector<shared_ptr<Automaton>> selectParents(vector<shared_ptr<Automaton>> &generation);
vector<shared_ptr<Automaton>> selectParentsNew(vector<shared_ptr<Automaton>> &generation);

Automaton findFittest(vector<Automaton> &generation);
shared_ptr<Automaton> findFittest(vector<shared_ptr<Automaton>> &generation);

void testAutomatonConstructor();

void testAutomatonVector();

void testAutomatonPointerVector();

void testAutomatonSmartPointerVector();

void testAutomatonBreeding();

void testRulePrint();

void testGKLRule();

void testDensity();

void testArbitraryRule();