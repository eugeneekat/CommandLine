#pragma once
#include "MyParser.h"
#include "DiscManager.h"
#include "MyLogger.h"
#include <memory>

class Interpritator
{
private:
	Singleton
	Interpritator()
	{
		//Инициализация парсера
		this->uptrParser.reset(new MyParser);
		//Инициализация исполнителя
		this->uptrExcecuter.reset(new DiscManager);
		//Инициализация логгера
		this->uptrLogger.reset(new MyLogger);
	}
	Interpritator(Interpritator const&)throw();
	Interpritator& operator= (Interpritator const&)const throw();
	//Модули
	std::unique_ptr<IParser>uptrParser;
	std::unique_ptr<IExecutable>uptrExcecuter;
	std::unique_ptr<ILogger>uptrLogger;
	
public:
	~Interpritator()throw()	{}
	//Получение ссылки на класс Interpritator
	static Interpritator & GetInstance()
	{
		static Interpritator interpritator;
		return interpritator;
	}
	/*Метод выполнения команды в соответствии с выбранной
	стратегией парсинга и класса исполнителя*/
	void Execute(const string & strCmd)
	{
		try
		{
			//Парсинг
			vector<string>args = uptrParser->Algorithm(strCmd);
			//Выполнение
			this->uptrExcecuter->Execute(args);
		}
		//Перехват ошибок и логгирование
		catch (const MyException & ex)
		{
			std::cout << ex << std::endl;
			this->uptrLogger->Log(ex.what());
		}
		catch (const std::exception & ex)
		{
			std::cout << ex.what() << std::endl;
			this->uptrLogger->Log(ex.what());
		}
		catch (...)
		{
			std::cout << "Unknown exception" << std::endl;
		}
	}
};

