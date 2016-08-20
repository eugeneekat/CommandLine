#pragma once
#include <string>
#include <fstream>

//Абстрактный класс логгер
class ILogger
{
public:
	ILogger() {}
	virtual ~ILogger() {}
	virtual void Log(const std::string & msg) = 0;
};