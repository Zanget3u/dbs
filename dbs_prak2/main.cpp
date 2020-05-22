#include <iostream>
#include "database.hpp"

int main(int argc, char* argv[])
{
	Database db;
	
	db.login("194.94.121.224", "15434", "dbs029", "dbs029", "Y19?;Y><");

	db.deleteAllEntries();	
	std::cout << "--------------------------------------" << std::endl;
	
	db.insertEntriesFromFile("data/data1");	
	db.printNumberOfEntries();
	std::cout << "--------------------------------------" << std::endl;
	
	db.insertEntriesFromFile("data/data2");
	db.printNumberOfEntries();
	std::cout << "--------------------------------------" << std::endl;
	
	db.deleteEntriesFromFile("data/data2");
	db.printNumberOfEntries();
	std::cout << "--------------------------------------" << std::endl;	

	db.insertEntriesFromFile("data/data3");
	db.printNumberOfEntries();
	std::cout << "--------------------------------------" << std::endl;
	
	db.logout();
	
	std::cin.get();
	return 0;
}