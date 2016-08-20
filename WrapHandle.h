#pragma once
#include <direct.h>
#include <io.h>

//Обертка дискриптора поиска для умных указателей
class WrapHandle
{
private:
	intptr_t handle;
public:
	//Можно _findfirst передавать в конструкторе
	WrapHandle(const intptr_t & h = -1) : handle(h) {}
	//При уничтожении объекта дискриптор поиска будет закрыт
	~WrapHandle()
	{
		if(handle != -1)
			_findclose(this->handle);
	}
	//Можно _findfirst передавать через оператор присваивания
	WrapHandle & operator=(const intptr_t & h)
	{
		//Если открыт дискриптор то закрываем
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
