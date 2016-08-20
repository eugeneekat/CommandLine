// exam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interpritator.h"

int main()
{
	//Инициализация интерпритатора и получение адреса на него
	try
	{
		Interpritator & interpritator = Interpritator::GetInstance();
		string command;
		interpritator.Execute("scd");
		while (std::getline(std::cin, command) && command != "exit")
		{
			interpritator.Execute(command);
			interpritator.Execute("scd");
		}
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception" << std::endl;
	}
    return 0;
}

