#include <iostream>
#include "consoleMenu.hpp"

int main(int argc, char* argv[])
{
	ConsoleMenu menu;
	menu.startMenu();
	
	std::cin.get();
	return 0;
}