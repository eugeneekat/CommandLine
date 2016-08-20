#include "stdafx.h"
#include "DiscManager.h"

//���������� ���� ������� (���������� � ������������)
void DiscManager::FillMethodMap()throw()
{
	//���������� ����� ������� ��� ����������
	this->voidMethodMap["scd"] = &DiscManager::ShowCurrentDir;
	this->voidMethodMap["dir"] = &DiscManager::ShowDir;
	this->voidMethodMap["help"] = &DiscManager::Help;
	//���������� ����� ������� � 1 ����������
	this->oneArgsMethodMap["cd"] = &DiscManager::ChangeDir;
	this->oneArgsMethodMap["read"] = &DiscManager::ReadFile;
	this->oneArgsMethodMap["crdir"] = &DiscManager::CreateDir;
	this->oneArgsMethodMap["crfil"] = &DiscManager::CreateFile;
	this->oneArgsMethodMap["delete"] = &DiscManager::Delete;
	//���������� ����� ������� � 2 �����������
	this->twoArgsMethodMap["copy"] = &DiscManager::Copy;
}

/*��������������� ������*/
//����������� ���� ��� ������� � ������������ ������ � ���
std::ostringstream DiscManager::PrepareDir(const string & dir)const throw()
{
	std::ostringstream os;
	if (dir[dir.length() - 1] == '\\')
		os << dir.substr(0, dir.length() - 1);
	else
		os << dir;
	return os;
}
//��������� ���������� �� ���������� �� ������� ����
bool DiscManager::IsDirExist(const string & dir)const throw()
{
	std::ostringstream os(this->PrepareDir(dir));
	os << "\\*.*";
	_finddata_t fd = { 0 };
	intptr_t handle = _findfirst(os.str().c_str(), &fd);
	if (handle == -1)
	{
		_findclose(handle);
		return false;
	}
	_findclose(handle);
	return true;
}
//��������� ���������� �� ���� �� ������� ����
bool DiscManager::IsFileExist(const string & file)const throw()
{
	std::ifstream os(file, std::ios::binary);
	bool ret = false;
	if (os.is_open())
		ret = true;
	os.close();
	return ret;
}
//�������� ����
void DiscManager::CopyFile(const string & source, const string & dest)const
{
	std::ostringstream path;
	path << dest << "\\" << source.substr(source.rfind("\\") + 1);
	//��� ������� ����������� ���� ��� � ����
	if (source == path.str())
		return;
	//���� � ����� ���� ���� � ����� ���������
	_finddata_t fd = { 0 };
	std::unique_ptr<WrapHandle>handle(new WrapHandle());
	*handle = _findfirst(path.str().c_str(), &fd);
	while (*handle != -1)
	{
		static int i = 0;
		i++;
		std::ostringstream newPath;
		newPath << dest << source.substr(source.rfind("\\"), source.rfind(".") - source.rfind("\\")) << "_" << i << source.substr(source.rfind("."));
		*handle = _findfirst(newPath.str().c_str(), &fd);
		if (*handle == -1)
		{
			i = 0;
			path.str(newPath.str());
		}
	}
	std::ifstream inFile(source, std::ios::binary);
	std::ofstream outFile(path.str(), std::ios::binary);
	if (!inFile.is_open() || !outFile.is_open())
	{
		std::ostringstream os;
		os << "time: " << time(0) << " Error copy file from: " << source << " to: " << path.str() << " ---- Can't open file.";
		throw MyException(os.str(), "Error copy file: can't open file.");
	}
	while (!inFile.eof())
	{
		std::string strBuf(4096, '\0');
		inFile.read(&strBuf[0], strBuf.size());
		size_t sizeRead = inFile.gcount();
		if (sizeRead)
		{
			strBuf.resize(sizeRead);
			outFile << strBuf;
		}
	}
	inFile.close();
	outFile.close();
}
//�������� ����������
void DiscManager::CopyDir(const string & source, const string & dest)const
{
	static std::ostringstream exceptionLog;
	static int recCount = 0;
	std::ostringstream osDest;
	osDest << dest << "\\" << source.substr(source.rfind('\\') + 1);

	this->CreateDir(osDest.str());

	std::ostringstream osSource;
	osSource << source << "\\*.*";
	_finddata_t fdSource = { 0 };
	std::unique_ptr<WrapHandle>handleSource(new WrapHandle()); 
	*handleSource = _findfirst(osSource.str().c_str(), &fdSource);
	if (*handleSource == -1)
	{
 		std::ostringstream os;
		os << "Time: " << time(0) << " Error handle for: " << source;
		throw MyException(os.str(), "Error handle");
	}		
	do
	{
		if (fdSource.name == string(".") || fdSource.name == string(".."))
			continue;
		std::ostringstream newSource;
		newSource << source << "\\" << fdSource.name;
		try
		{
			if (fdSource.attrib & _A_SUBDIR)
			{
				recCount++;
				try
				{
					this->CopyDir(newSource.str(), osDest.str());
				}
				catch (const MyException & ex)
				{
					std::cout << ex << std::endl;
					exceptionLog << ex.what() << "\n";
				}
				recCount--;
			}
			else
				this->CopyFile(newSource.str(), osDest.str());
		}
		catch (const MyException & ex)
		{
			std::cout << ex << std::endl;
			exceptionLog << ex.what() << "\n";
		}
	} while (!_findnext(*handleSource, &fdSource));
	if (!recCount && exceptionLog.str().length())
	{
		MyException ex(exceptionLog.str());
		exceptionLog.str("");
		throw ex;
	}	
}
//������� ���������� �� ��������� ����
void DiscManager::DeleteDir(const string & dir)const
{
	int error = _rmdir(dir.c_str());
	if (error)
	{
		std::ostringstream os;
		os << "Time: " << time(0) << " Error deleting dir: " << dir;
		throw MyException(os.str(), "Error deleting dir!");
	}
}
//������� ���������� ����������
void DiscManager::DeleteDirRec(const string & dir)const
{
	static std::ostringstream exceptionLog;
	static int recCount = 0;
	std::ostringstream osDir;
	osDir << dir << "\\*.*";
	_finddata_t fd = { 0 };
	std::unique_ptr<WrapHandle>handle(new WrapHandle());
	*handle = _findfirst(osDir.str().c_str(), &fd);
	do
	{
		if (fd.name == string(".") || fd.name == string(".."))
			continue;
		std::ostringstream newDir;
		newDir << dir << "\\" << fd.name;
		try
		{
			if (fd.attrib & _A_SUBDIR)
			{
				recCount++;
				try
				{
					this->DeleteDirRec(newDir.str());
				}
				catch (const MyException & ex)
				{
					std::cout << ex << std::endl;
					exceptionLog << ex.what() << "\n";
				}
				recCount--;
				this->DeleteDir(newDir.str());
			}
			else
				this->DeleteFile(newDir.str());
		}
		catch (const MyException & ex)
		{
			std::cout << ex << std::endl;
			exceptionLog << ex.what() << "\n";
		}
	} while (!_findnext(*handle, &fd));
	if (!recCount)
	{
		if (exceptionLog.str().length())
		{
			exceptionLog << "Time: " << time(0) << " Error deleting dir: " << dir << "\n";
			MyException ex(exceptionLog.str());
			exceptionLog.str("");
			throw ex;
		}
		this->DeleteDir(dir);
	}
}

/*�������� ������*/
//�������
void DiscManager::Help()const throw()
{
	std::ostringstream os;
	os << "--Comand--\t\t--Result--\t\t--Example--\n"
		<< "dir\t\tView Directory Contents\t\tdir\n"
		<< "cd\t\tChange current position\n\t\tin directory or disc\t\tcd C:\\ D:\\MyDir\n"
		<< "read\t\tRead file\t\t\tread C:\\ReadFile.txt\n"
		<< "crdir\t\tCreate directory\t\tcrdir C:\\MyDir\n"
		<< "crfil\t\tCreate file\t\t\tcrfil C:\\MyFile.txt\n"
		<< "delete\t\tDelete directiry or file\tdelete C:\\MyFile.txt\n\t\t\t\t\t\tdelete D:\\MyDir\n"
		<< "copy\t\tCopy directory or file\t\tcopy C:\\MyFile.txt D:\\MyFolder\\MyFile.txt\n\t\t\t\t\t\tcopy C:\\MyDir D:\\MySecondDir - (Warning there must be MySecondDir)";
	std::cout << os.str() << std::endl;
}
//���������� ���������� �������� ��������
void DiscManager::ShowDir()const throw()
{
	std::ostringstream os;
	os << this->currentDirectory << "\\*.*";
	_finddata_t fd = { 0 };
	std::unique_ptr<WrapHandle>handle(new WrapHandle());
	*handle = _findfirst(os.str().c_str(), &fd);
	if (*handle == -1)
	{
		std::ostringstream ex;
		ex << "Time: " << time(0) << ", Error show dir in: " << this->currentDirectory << "! Directory unavailable.\n";
		throw MyException(ex.str(), "Directory unavailable(probably was deleted) please change directory\n");
	}
	do
	{
		if (fd.name == string(".") || fd.name == string(".."))
			continue;
		std::cout << this->currentDirectory << "\\" << fd.name << std::endl;
	} while (!_findnext(*handle, &fd));
}
//������ ������� ��������� � ��������
void DiscManager::ChangeDir(const string & dir)const
{
	std::ostringstream os(this->PrepareDir(dir).str());
	if (this->IsDirExist(os.str()))
		this->currentDirectory = os.str();
	else
	{
		std::ostringstream os;
		os << "time: " << time(0) << " Error change dir to: " << dir << " ----- dir not exist.";
		throw MyException(os.str(), "Error change dir");
	}

}
//������ ����� � ����� �� �������
void DiscManager::ReadFile(const string & path)const
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "time: " << time(0) << " Error read file: " << path << " ----- file not exist.";
		throw MyException(os.str(), "Error read file");
	}
	while (!file.eof())
	{
		std::string strBuf(4096, '\0');
		file.read(&strBuf[0], strBuf.size());
		size_t sizeRead = file.gcount();
		if (sizeRead)
		{
			strBuf.resize(sizeRead);
			std::cout << strBuf;
		}
	}
	std::cout << std::endl;
	file.close();
}
//������� ����
void DiscManager::CreateFile(const string & path)const
{
	//���� ����� ���� ��� ���� �� ������� ����������
	if (this->IsFileExist(path))
	{
		std::ostringstream os;
		os << "time: " << time(0) << " Error create file: " << path << " ----- file already exist.";
		throw MyException(os.str(), "Error create file: file already exist.");
	}
	std::ofstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "time: " << time(0) << " Error create file: " << path << " ----- can't create file.";
		throw MyException(os.str(), "Error create file: fatal error.");
	}
	file.close();
}
//�������� ����/����������
void DiscManager::Copy(const string & pathSource, const string & pathDest)const
{
	//����������� ���� ����������
	std::ostringstream osDest(this->PrepareDir(pathDest));
	//���� ����� � ������ ������������ ����������
	if (this->IsDirExist(pathSource) && this->IsDirExist(pathDest))
	{
		//����������� ���� ���������
		std::ostringstream osSource(this->PrepareDir(pathSource));
		//�������� ���������� ���������� ����� � ���������� ������
		this->CopyDir(osSource.str(), osDest.str());
	}
	//���� ����� ����, � ������ ���������� �� �������� ���� ����� � ���������� �����
	else if (this->IsFileExist(pathSource) && this->IsDirExist(pathDest))
		this->CopyFile(pathSource, osDest.str());
	//����� ������������ ����
	else
	{
		std::ostringstream os;
		os << "Time: " << time(0) << " Copy error --- Incorrect copy from " << pathSource << " to " << pathDest;
		throw MyException(os.str(), "Copy error: incorrect copy context");
	}
}
//������� ����/����������
void DiscManager::Delete(const string & path)const
{
	if (this->IsDirExist(path))
		this->DeleteDirRec(path);	
	else if (this->IsFileExist(path))
		this->DeleteFile(path);
	else
	{
		std::ostringstream os;
		os << "Time: " << time(0) << " Delete error ---- incorrect path: " << path;
		throw MyException(os.str(), "Delete error: incorrect path!");
	}
}

//����� ���������� �������
void DiscManager::Execute(const vector<string> & args)
{
	/*���������� ���������� ����������, ��������� � ������,
	� ���� ���� ����� ����� - ��������� ���, ���� ���� ����������� ����������*/
	switch (args.size())
	{
	case NoArguments:
		if (this->voidMethodMap.find(args[0]) == this->voidMethodMap.cend())
			goto exceptionLabel;
		(this->*voidMethodMap[args[0]])();
		break;
	case OneArgument:
		if (this->oneArgsMethodMap.find(args[0]) == this->oneArgsMethodMap.cend())
			goto exceptionLabel;
		(this->*oneArgsMethodMap[args[0]])(args[1]);
		break;
	case TwoArgunemts:
		if (this->twoArgsMethodMap.find(args[0]) == this->twoArgsMethodMap.cend())
			goto exceptionLabel;
		(this->*twoArgsMethodMap[args[0]])(args[1], args[2]);
		break;
	default:
	exceptionLabel:
		std::ostringstream os;
		os << "Time: " << time(0) << " Error: Incorrect command";
		throw MyException(os.str(), "Incorrect command");
	}
}