#pragma once
#include "ILogger.h"
//Реализация моего логгера
class MyLogger : public ILogger
{
private:
	MyLogger(const MyLogger &);
	MyLogger & operator=(MyLogger const&);
public:
	MyLogger() {}
	virtual ~MyLogger() {}
	virtual void Log(const std::string & msg)
	{
		std::ofstream fs;
		fs.open("MyLogger.log", std::ios::app);
		if (fs.is_open())
		{
			fs << msg << "\n";
			fs.close();
		}
	}
};
