#include <iostream>
#include "database.hpp"

int main(int argc, char* argv[])
{
	Database db;
	db.login("194.94.121.224", "15434", "dbs029", "dbs029", "Y19?;Y><");
	
	db.deleteAllEntries();
	db.begin();
	db.insertEntry("H1", "Henkel", "47807", "Duesseldorf");
	db.commit();
	db.findhnr("H1");
	db.selectAllEntries();

	db.logout();
	
	std::cin.get();
	return 0;
}