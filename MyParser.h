#pragma once
#include "IParser.h"

//Реализация моего парсера
class MyParser : public IParser
{
private:
	MyParser(const MyParser &);
	MyParser & operator=(MyParser const&);
public:
	MyParser() {}
	~MyParser() {}
	std::vector<string> Algorithm(const string & strToParse)
	{
		vector<string>args;
		string cmdStr = strToParse;
		args.push_back(cmdStr.substr(0, cmdStr.find(" ")));
		if (cmdStr.find(" ") == string::npos)
			return args;
		cmdStr.erase(0, cmdStr.find(" ") + 1);
		if (cmdStr.rfind(":\\") - 1)
		{
			args.push_back(cmdStr.substr(0, cmdStr.rfind(":\\") - 2));
			cmdStr.erase(0, cmdStr.rfind(":\\") - 1);
		}
		if (cmdStr != "")
			args.push_back(cmdStr);
		return args;
	}
};
