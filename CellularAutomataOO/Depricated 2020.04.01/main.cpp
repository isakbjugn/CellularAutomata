// CellularAutomataOO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Automaton.h"
#include "Evolution.h"


using namespace std::chrono;

int main()
{
	setlocale(LC_ALL, "Norwegian");
    //std::cout << "Kari er best!\n";

	//testAutomatonConstructor();
	//testAutomatonVector();
	//testAutomatonPointerVector();
	//testAutomatonSmartPointerVector();
	//testAutomatonBreeding();
	//testRulePrint();
	//testGKLRule();
	
	//Start timer
	auto start = high_resolution_clock::now();

	//evolution(100, 2);
	evolutionParallel(100, 15);
	//testArbitraryRule();

	// End timer
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << duration.count() << std::endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
