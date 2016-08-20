#pragma once
#include <exception>
#include <string>
#include <iostream>
class MyException : public std::exception
{
protected:
	std::string userMsg;
public:
	MyException(const std::string & loggerMsg = (""), const std::string & usrStr = ("")) : exception(loggerMsg.c_str())
	{
		this->userMsg = usrStr;
	}
	virtual ~MyException() {}
	//Отображение ошибки для пользователя
	friend std::ostream & operator<<(std::ostream & os, const MyException & ex)
	{
		os << ex.userMsg;
		return os;
	}
};