#include "database.hpp"
#include <iostream>
#include "entry.hpp"

//--private
std::string Database::sqlStringConvert(const std::string& unconvertedString)
{
	std::string convertedString = "'" + unconvertedString + "'";
	return convertedString;
}

int Database::executeCommand(const std::string& command)
{
	PQclear(res);
	res = PQexec(conn, command.c_str());

	//Fehlerüberprüfung
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::cout << "-> " << command << " - ist fehlgeschlagen!" << std::endl;
		std::cout << PQerrorMessage(conn) << std::endl;
		return 1;
	}
	else
	{
		std::cout << "-> " << command << " wurde erfolgreich ausgefuehrt!" << std::endl;
	}
	
	return 0;
}

int Database::executeSelectWithIO(const std::string& selectCommand)
{
	PQclear(res);
	res = PQexec(conn, selectCommand.c_str());

	//Fehlerüberprüfung
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		std::cout << "-> " << selectCommand << " - ist fehlgeschlagen!" << std::endl;
		std::cout << PQerrorMessage(conn) << std::endl;
		return 1;
	}
	else
	{
		//std::cout << "-> " << selectCommand << " wurde erfolgreich ausgefuehrt!" << std::endl;
	}

	// Ausgabe Ergebnisse
	for (int i = 0; i < PQntuples(res); i++) {
		printf("%2d. %s\n", i + 1, PQgetvalue(res, i, 0));
	}

	return 0;
}

int Database::executeSelectWithoutIO(const std::string& selectCommand)
{
	PQclear(res);
	res = PQexec(conn, selectCommand.c_str());

	//Fehlerüberprüfung
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		return 1;
	}

	if(PQntuples(res) == 0)
	{
		return 2;
	}
	
	return 0;
}

int Database::begin()
{
	return this->executeCommand("BEGIN");
}

int Database::commit()
{
	return this->executeCommand("COMMIT");
}

int Database::rollback()
{
	return this->executeCommand("ROLLBACK");
}

//--public
int Database::login(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password)
{
	std::string connstr;
	
	//Create String
	connstr += "host=";
	connstr += host;
	connstr += " port=";
	connstr += port;
	connstr += " dbname=";
	connstr += dbname;
	connstr += " user=";
	connstr += user;
	connstr += " password=";
	connstr += password;

	conn = PQconnectdb(connstr.c_str());

	//Fehlerüberprüfung
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		std::cout << "Verbindung zur Datenbank fehlgeschlagen!" << std::endl;
		std::cout << PQerrorMessage(conn) << std::endl;
		PQfinish(conn);
		return 1;
	}
	else
	{
		std::cout << "Erfolgreich mit Datenbank verbunden!" << std::endl;
	}
	
	return 0;
}

void Database::logout()
{
	PQclear(res);
	PQfinish(conn);
	std::cout << "Erfolgreich aus Datenbank ausgeloggt!" << std::endl;
}

int Database::findhnr(const std::string& hnr)
{
	std::string command = "SELECT * FROM hersteller WHERE hnr = ";
	command += sqlStringConvert(hnr);
	command += ";";

	int result = this->executeSelectWithoutIO(command);
	
	if (result == 0)	
		std::cout << "Der Eintrag " << hnr << " existiert!" << std::endl;	
		
	else if (result == 1)	
		std::cout << "Es gab einen Fehler bei der Abfrage des Eintrags: " << hnr << std::endl;
			
	else if(result == 2)	
		std::cout << "Der Eintrag " << hnr << " existiert nicht!" << std::endl;
		
	return result;
}

int Database::insertEntry(const std::string& hnr, const std::string& name, const std::string& plz, const std::string& ort)
{
	std::string command = "INSERT INTO hersteller(hnr, name, plz, ort)";
	command += "VALUES(";
	command += this->sqlStringConvert(hnr);
	command += ", ";
	command += this->sqlStringConvert(name);
	command += ", ";
	command += this->sqlStringConvert(plz);
	command += ", ";
	command += this->sqlStringConvert(ort);
	command += ");";

	int result = this->executeCommand(command);	
	return result;
}

int Database::deleteEntry(const std::string& hnr)
{
	std::string command = "SELECT * FROM hersteller WHERE hnr = ";
	command += this->sqlStringConvert(hnr);
	command += " ;";

	int result = this->executeSelectWithoutIO(command);

	if(result == 0)
	{
		command = "DELETE FROM hersteller WHERE hnr = ";
		command += this->sqlStringConvert(hnr);
		command += " ;";

		result = this->executeCommand(command);
		return result;
	}

	return result;
}

int Database::deleteAllEntries()
{
	return this->executeCommand("DELETE FROM hersteller;");
}

int Database::selectAllEntries()
{
	return this->executeSelectWithIO("SELECT * FROM hersteller;");
}

int Database::insertEntriesFromFile(const std::string& filepath)
{
	const auto entryVector = getDataFromFile(filepath);	
	int entriesInserted = 0;	
	bool insertError = false;
	
	this->begin();
	
	for(entry e : *entryVector)
	{
		int entryExists = this->findhnr(e.hnr);
		
		if(entryExists == 2) //If there is no entry with the same hnr
		{			
			int result = this->insertEntry(e.hnr, e.name, e.plz, e.ort);

			if (result == 0)			
				entriesInserted++;
			
			else if(result == 1)			
				insertError = true;
		}		
	}
	
	if (insertError == true)
	{
		this->rollback();
		entriesInserted = 0;
		std::cout << "Transaktion wurde abgebrochen!" << std::endl;
	}
	else if (insertError == false)
	{
		this->commit();
	}
	
	std::cout << "Datensaetze: " << entryVector->size() << " / davon importiert: " << entriesInserted << std::endl;

	delete entryVector;
	return 0;
}

void Database::printNumberOfEntries()
{
	PQclear(res);
	std::string command = "SELECT * FROM hersteller;";	
	res = PQexec(conn, command.c_str());

	//Fehlerüberprüfung
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		std::cout << "Fehler bei Select-Abfrage!" << std::endl;
	}
	else
	{
		std::cout << "Anzahl der Tabellensaetze: " << PQntuples(res) << std::endl;
	}
}

int Database::deleteEntriesFromFile(const std::string& filepath)
{
	const auto entryVector = getDataFromFile(filepath);
	int entriesDeleted = 0;
	bool deleteError = false;

	this->begin();
	
	for (entry e : *entryVector)
	{		
		int result = this->deleteEntry(e.hnr);
		
		if (result == 0)
			entriesDeleted++;
		else if (result == 1)
			deleteError = true;
	}

	if (deleteError == true)
	{
		this->rollback();
		entriesDeleted = 0;
		std::cout << "Transaktion wurde abgebrochen!" << std::endl;
	}
	else if (deleteError == false)
	{
		this->commit();
	}
	
	std::cout << "Datensaetze: " << entryVector->size() << " / davon geloescht: " << entriesDeleted << std::endl;

	delete entryVector;
	return 0;
}