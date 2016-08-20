#pragma once
#include "IExecutable.h"
#include "MyException.h"
#include "WrapHandle.h"
#include <map>
#include <sstream>
#include <fstream>
#include <ctime>
#include <memory>

class DiscManager : public IExecutable
{
private:
	//Текущее положение в каталоге
	mutable string currentDirectory;
protected:
	enum MethodArgumentsCount {NoArguments = 1, OneArgument = 2, TwoArgunemts = 3};

	/*Карты методов (без аргументов, с 1 аргументом и с 2 аргументами)*/
	std::map<string, void(DiscManager::*)()const> voidMethodMap;
	std::map<string, void(DiscManager::*)(const string & arg)const> oneArgsMethodMap;
	std::map<string, void(DiscManager::*)(const string & arg1, const string & arg2)const> twoArgsMethodMap;
	
	//Заполнение карт методов
	virtual void FillMethodMap()throw();

	/*Вспомогательные методы*/
	
	//Форматирует путь для работы с ним
	std::ostringstream PrepareDir(const string & dir)const throw();
	
	//Проверяет существует ли директория по данному пути
	bool IsDirExist(const string & dir)const throw();
	
	//Проверяет существует ли файл по данному пути
	bool IsFileExist(const string & file)const throw();
	
	//Копирует файл
	void CopyFile(const string & source, const string & dest)const;
	
	//Копирует директорию
	void CopyDir(const string & source, const string & dest)const;

	//Удаляет файл
	void DeleteFile(const string & file)const
	{
		int error = remove(file.c_str());
		if (error)
		{
			std::ostringstream os;
			os << "Time: " << time(0) << " Error deleting file: " << file;
			throw MyException(os.str(), "Error deleting file");
		}
	}

	//Удаляет директорию по заданному пути
	void DeleteDir(const string & dir)const;

	//Удаляет директорию рекурсивно
	void DeleteDirRec(const string & dir)const;

	/*Основные методы*/
	
	//Справка
	void Help()const throw();
	
	//Отображает текущее положение в каталоге
	void ShowCurrentDir()const throw()
	{
		std::cout << this->currentDirectory << "\\ >";
	}
	
	//Отображает содерживое текущего каталога
	void ShowDir()const;
	
	//Меняет текущее положение в каталоге
	void ChangeDir(const string & dir)const;
	
	//Чтение файла и вывод на консоль
	void ReadFile(const string & path)const;
	
	//Создает директорию
	void CreateDir(const string & path)const
	{
		int err = _mkdir(path.c_str());
		if (err)
		{
			std::ostringstream os;
			os << "time: " << time(0) << " Error create dir: " << path << " ----- dir already exist or path not available.";
			throw MyException(os.str(), "Error create dir");
		}
	}
	
	//Создает файл
	void CreateFile(const string & path)const;
	
	//Копирует файл/директорию
	void Copy(const string & pathSource, const string & pathDest)const;
	
	//Удаляет файл/директорию
	void Delete(const string & path)const;

public:
	DiscManager(const string & dir = string("C:"))throw()
	{
		this->currentDirectory = dir;
		this->FillMethodMap();
	}
	virtual ~DiscManager()throw() {};
	//Метод исполняющий команду
	virtual void Execute(const vector<string> & args);
};
