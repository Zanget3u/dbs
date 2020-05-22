#pragma once

#include <libpq-fe.h>
#include <string>

class Database
{
private:
	PGconn* conn = nullptr;
	PGresult* res = nullptr;
	std::string sqlStringConvert(const std::string& unconvertedString);
	int executeCommand(const std::string& command);
	int executeSelectWithIO(const std::string& selectCommand);
	int executeSelectWithoutIO(const std::string& selectCommand);
	
public:
	int login(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password);
	void logout();
	int begin();
	int commit();
	int rollback();
	int findhnr(const std::string& hnr);
	int insertEntry(const std::string& hnr, const std::string& name, const std::string& plz, const std::string& ort);
	int deleteAllEntries();
	int selectAllEntries();
};