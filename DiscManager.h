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
	//������� ��������� � ��������
	mutable string currentDirectory;
protected:
	enum MethodArgumentsCount {NoArguments = 1, OneArgument = 2, TwoArgunemts = 3};

	/*����� ������� (��� ����������, � 1 ���������� � � 2 �����������)*/
	std::map<string, void(DiscManager::*)()const> voidMethodMap;
	std::map<string, void(DiscManager::*)(const string & arg)const> oneArgsMethodMap;
	std::map<string, void(DiscManager::*)(const string & arg1, const string & arg2)const> twoArgsMethodMap;
	
	//���������� ���� �������
	virtual void FillMethodMap()throw();

	/*��������������� ������*/
	//����������� ���� ��� ������ � ���
	std::ostringstream PrepareDir(const string & dir)const throw();
	//��������� ���������� �� ���������� �� ������� ����
	bool IsDirExist(const string & dir)const throw();
	//��������� ���������� �� ���� �� ������� ����
	bool IsFileExist(const string & file)const throw();
	//�������� ����
	void CopyFile(const string & source, const string & dest)const;
	//�������� ����������
	void CopyDir(const string & source, const string & dest)const;
	//������� ����
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
	//������� ���������� �� ��������� ����
	void DeleteDir(const string & dir)const;
	//������� ���������� ����������
	void DeleteDirRec(const string & dir)const;

	/*�������� ������*/
	//�������
	void Help()const throw();
	//���������� ������� ��������� � ��������
	void ShowCurrentDir()const throw()
	{
		std::cout << this->currentDirectory << "\\ >";
	}
	//���������� ���������� �������� ��������
	void ShowDir()const throw();
	//������ ������� ��������� � ��������
	void ChangeDir(const string & dir)const;
	//������ ����� � ����� �� �������
	void ReadFile(const string & path)const;
	//������� ����������
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
	//������� ����
	void CreateFile(const string & path)const;
	//�������� ����/����������
	void Copy(const string & pathSource, const string & pathDest)const;
	//������� ����/����������
	void Delete(const string & path)const;

public:
	DiscManager(const string & dir = string("C:"))throw()
	{
		this->currentDirectory = dir;
		this->FillMethodMap();
	}
	virtual ~DiscManager()throw() {};
	//����� ����������� �������
	virtual void Execute(const vector<string> & args);
};