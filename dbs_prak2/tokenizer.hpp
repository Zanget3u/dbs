#pragma once

#include <string>
#include "liste.hpp"

using namespace std;

class Tokenizer {
private:
	unsigned int _pos;
	Liste<string> *_tokens;
public:
	Tokenizer(string data, string separators);
	~Tokenizer();
	int countTokens();
	string nextToken();
	bool hasMoreTokens();
};

Tokenizer::Tokenizer(string data, string sep)
{
	_tokens = new Liste<string>(4);
	_pos = 0;

	string::size_type beg, end;
	beg = data.find_first_not_of(sep, 0);
	end = data.find_first_of(sep, beg);

	while (string::npos != beg || string::npos != end) {
		string s = data.substr(beg, end - beg);
		_tokens->append(s);
		beg = data.find_first_not_of(sep, end);
		end = data.find_first_of(sep, beg);
	}
}

Tokenizer::~Tokenizer()
{
	delete _tokens;
}

int Tokenizer::countTokens() {
	return _tokens->size();
}

string Tokenizer::nextToken() {
	return _tokens->getValueAt(_pos++);
}

bool Tokenizer::hasMoreTokens() {
	return _pos < _tokens->size();
}