#pragma once
#include <exception>
#include <string>
#include <iostream>
//Мой класс исключения - населуется от стандартного
class MyException : public std::exception
{
protected:
	//Расширяет на ещё 1 сообщение для пользователя(Второе сообщение для логгера хранится в поле стандратного исключения)
	std::string userMsg;
public:
	MyException(const std::string & loggerMsg = (""), const std::string & usrStr = ("")) : exception(loggerMsg.c_str())
	{
		this->userMsg = usrStr;
	}
	virtual ~MyException() {}
	friend std::ostream & operator<<(std::ostream & os, const MyException & ex)
	{
		os << ex.userMsg;
		return os;
	}
};
