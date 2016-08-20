#pragma once
#include "MyParser.h"
#include "DiscManager.h"
#include "MyLogger.h"
#include <memory>

class Interpritator
{
private:
	//Закрытый конструктор - Singleton
	Interpritator()
	{
		//Инициализация алгоритма нарезания
		this->uptrParser.reset(new MyParser);
		//Инициализация класса - исполнителя
		this->uptrExcecuter.reset(new DiscManager);

		this->uptrLogger.reset(new MyLogger);
	}
	//Запрещены операторы копирования и присваивания
	Interpritator(Interpritator const&)throw();
	Interpritator& operator= (Interpritator const&)const throw();
	//Модули интерпритатора
	std::unique_ptr<IParser>uptrParser;
	std::unique_ptr<IExecutable>uptrExcecuter;
	std::unique_ptr<ILogger>uptrLogger;
	
public:
	~Interpritator()throw()	{}
	//Статический метод - возвращает ссылку на статический класс
	static Interpritator & GetInstance()
	{
		static Interpritator interpritator;
		return interpritator;
	}
	/*Метод выполнения команды в соответствии с выбранным
	алгоритмом парсера и класса исполнителя*/
	void Execute(const string & strCmd)
	{
		try
		{
			//Парсинг
			//vector<string>args = this->parse->Algorithm(strCmd);
			vector<string>args = uptrParser->Algorithm(strCmd);
			//Исполнение
			this->uptrExcecuter->Execute(args);
		}
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

