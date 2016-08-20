#pragma once
#include <direct.h>
#include <io.h>

//������� ����������� ������ ��� ����� ����������
class WrapHandle
{
private:
	intptr_t handle;
public:
	//����� _findfirst ���������� � ������������
	WrapHandle(const intptr_t & h = -1) : handle(h) {}
	//��� ����������� ������� ���������� ������ ����� ������
	~WrapHandle()
	{
		if(handle != -1)
			_findclose(this->handle);
	}
	//����� _findfirst ���������� ����� �������� ������������
	WrapHandle & operator=(const intptr_t & h)
	{
		//���� ������ ���������� �� ���������
		if (this->handle != -1)
			_findclose(this->handle);
		this->handle = h;
		return *this;
	}
	operator intptr_t ()const throw()
	{
		return this->handle;
	}
};