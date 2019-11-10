#include "Pacman.h"
#include <iostream>

using namespace S2D;

int main(int argc, char* argv[]) 
{
	int userInput;
	cout << "Please Enter a Number! \n1) Pacman \n2) Exit Application" << endl;
	cin >> userInput;
	if (userInput == 1)
	{
		Pacman* Game = new Pacman(argc, argv);
	}
}

