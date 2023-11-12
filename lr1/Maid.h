#pragma once
#include "Address.h"
#include <string>

class Maid {
public:
	int id;
	wstring name;
	wstring surname;
	wstring patronymic;
	Address address;


	Maid(int id, wstring name, wstring surname, wstring patronymic, Address address);
	Maid();
	~Maid();
};