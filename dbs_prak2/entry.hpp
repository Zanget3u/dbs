#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "tokenizer.hpp"

struct entry
{
	std::string hnr;
	std::string name;
	std::string plz;
	std::string ort;
};

std::vector<entry>* getDataFromFile(const std::string& filepath)
{
	std::string line, token;
	std::ifstream file(filepath);
	auto entryVector = new std::vector<entry>;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			Tokenizer tokenizer(line, ";");
			entry e;

			e.hnr = tokenizer.nextToken(); //hnr			
			e.name = tokenizer.nextToken(); //name			
			e.plz = tokenizer.nextToken(); //plz			
			e.ort = tokenizer.nextToken(); //ort

			entryVector->push_back(e);
		}
		return entryVector;
	}
	else
	{
		std::cout << "Datei konnte nicht geoeffnet werden!" << std::endl;
		return nullptr;
	}
}

void printEntriesFromVector(const std::vector<entry>* entryVector)
{
	for (auto e : *entryVector)
	{
		std::cout << "hnr: " << e.hnr << std::endl;
		std::cout << "name: " << e.name << std::endl;
		std::cout << "plz: " << e.plz << std::endl;
		std::cout << "ort: " << e.ort << std::endl << std::endl;
	}
}