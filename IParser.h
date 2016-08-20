#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;
//����������� ����� ������
class IParser
{
public:
	IParser() {};
	virtual ~IParser() {};
	virtual std::vector<string> Algorithm(const string & strToParse) = 0;
};

