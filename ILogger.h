#pragma once
#include <string>
#include <fstream>

//Абстрактрый класс ILogger - описывает метод записи ошибки
class ILogger
{
public:
	ILogger() {}
	virtual ~ILogger() {}
	virtual void Log(const std::string & msg) = 0;
};
