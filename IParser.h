#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;
//Абстрактрый класс IParser - описывает стратегию парсинга блока комманд на отдельные части
class IParser
{
public:
	IParser() {};
	virtual ~IParser() {};
	virtual std::vector<string> Algorithm(const string & strToParse) = 0;
};

