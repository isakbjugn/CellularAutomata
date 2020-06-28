// CellularAutomataOO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Automaton.h"
#include "Evolution.h"


using namespace std::chrono;

int main()
{
	setlocale(LC_ALL, "Norwegian");
	std::cout << "New version: 2020-04-02-12-39\n";
	std::cout << "Evolution of Cellular Automata\n\n";

	//Start timer
	auto start = high_resolution_clock::now();

	//evolution(100, 100);
	//testArbitraryRule();
	testGKLRule();
	/*double value;
	std::cout << "Values: ";
	for (int i = 0; i < 11; i++)
	{
		value = 4 * double(std::pow(double(i)/10 - 0.5, 3.0));
		std::cout << value << " ";
	}
	std::cout << "\n";*/
	

	// End timer
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << "Milliseconds: " << duration.count() << std::endl;

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
