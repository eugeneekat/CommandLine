#pragma once
#include <string>
#include <fstream>

//����������� ����� ������
class ILogger
{
public:
	ILogger() {}
	virtual ~ILogger() {}
	virtual void Log(const std::string & msg) = 0;
};