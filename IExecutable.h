#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;
//Абстрактный класс - исполнитель
class IExecutable
{
public:
	IExecutable() {};
	virtual ~IExecutable() {};
	virtual void Execute(const vector<string> & args) = 0;
};

