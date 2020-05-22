#pragma once

#include <iostream>
#include "database.hpp"

class ConsoleMenu
{
private:
	void mainMenu();
	void exitMenu();
	Database* _db;
	int _menuOption = 0;
	bool _isLoggedIn = false;
	
public:
	ConsoleMenu();
	~ConsoleMenu();
	void startMenu();
};

inline ConsoleMenu::ConsoleMenu()
{
	_db = new Database();
}

inline ConsoleMenu::~ConsoleMenu()
{
	delete _db;
}

inline void ConsoleMenu::startMenu()
{
	std::cout << "------------------- Datenbankverwaltung -------------------" << std::endl;
	std::cout << "------------------------ Start Menu ------------------------" << std::endl;
	std::cout << "Login-Optionen: (1) Gespeicherter Nutzer | (2) Neuer Nutzer | (3) Beenden" << std::endl;

	std::cin >> _menuOption;

	if (_menuOption == 1)
	{
		_db->login("194.94.121.224", "15434", "dbs029", "dbs029", "Y19?;Y><");
		_isLoggedIn = true;
		this->mainMenu();
	}
	else if (_menuOption == 2)
	{
		std::string host, port, dbname, user, password;		
		std::cout << "Bitte nachfolgend die Daten eingeben" << std::endl;
		std::cout << "host: "; std::cin >> host;
		std::cout << "port: "; std::cin >> port;
		std::cout << "dbname: "; std::cin >> dbname;
		std::cout << "user: "; std::cin >> user;
		std::cout << "password: "; std::cin >> password;
		_db->login(host, port, dbname, user, password);
		_isLoggedIn = true;
		this->mainMenu();
	}
	else if (_menuOption == 3)
	{
		this->exitMenu();
	}
}

inline void ConsoleMenu::mainMenu()
{
	std::cout << std::endl << "------------------------ Main Menu ------------------------" << std::endl;
	std::cout << "Optionen: (1) Alle Daten loeschen | (2) Datensatz einfuegen | (3) Datensatz loeschen | (4) Beenden" << std::endl;
	
	std::cin >> _menuOption;

	if (_menuOption == 1)
	{
		_db->deleteAllEntries();
		this->mainMenu();
	}	
	else if (_menuOption == 2)
	{		
		std::cout << "(Einfuegen) Bitte Dateipfad angeben: ";
		std::string filename;
		std::cin >> filename;
		_db->insertEntriesFromFile(filename);
		_db->printNumberOfEntries();
		this->mainMenu();
	}
	else if (_menuOption == 3)
	{
		std::cout << "(Loeschen) Bitte Dateipfad angeben: ";
		std::string filename;
		std::cin >> filename;
		_db->deleteEntriesFromFile(filename);
		_db->printNumberOfEntries();
		this->mainMenu();
	}
	else if (_menuOption == 4)
	{
		this->exitMenu();
	}	
}

inline void ConsoleMenu::exitMenu()
{
	if (_isLoggedIn == true)
		_db->logout();
	std::cout << "Programm wird beendet!" << std::endl;
	std::cin.get();
}