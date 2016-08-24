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
		interpritator.GetCommand("scd");
		//Выполнение комманд
		while (std::getline(std::cin, command) && command != "exit")
		{
			interpritator.GetCommand(command);
			interpritator.GetCommand("scd");
		}
	}
	//Эти исключения ловят ошибки при инициализации интерпритатора(ошибки выполнения ловятся в самом интерпритаторе)
	catch (const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception" << std::endl;
	}
    return 0;
}

