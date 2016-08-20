#pragma once
#include "MyParser.h"
#include "DiscManager.h"
#include "MyLogger.h"
#include <memory>

class Interpritator
{
private:
	//�������� ����������� - Singleton
	Interpritator()
	{
		//������������� ��������� ���������
		this->uptrParser.reset(new MyParser);
		//������������� ������ - �����������
		this->uptrExcecuter.reset(new DiscManager);

		this->uptrLogger.reset(new MyLogger);
	}
	//��������� ��������� ����������� � ������������
	Interpritator(Interpritator const&)throw();
	Interpritator& operator= (Interpritator const&)const throw();
	//������ ��������������
	std::unique_ptr<IParser>uptrParser;
	std::unique_ptr<IExecutable>uptrExcecuter;
	std::unique_ptr<ILogger>uptrLogger;
	
public:
	~Interpritator()throw()	{}
	//����������� ����� - ���������� ������ �� ����������� �����
	static Interpritator & GetInstance()
	{
		static Interpritator interpritator;
		return interpritator;
	}
	/*����� ���������� ������� � ������������ � ���������
	���������� ������� � ������ �����������*/
	void Execute(const string & strCmd)
	{
		try
		{
			//�������
			//vector<string>args = this->parse->Algorithm(strCmd);
			vector<string>args = uptrParser->Algorithm(strCmd);
			//����������
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

