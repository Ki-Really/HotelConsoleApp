#include "Maid.h"


Maid::Maid(int id, wstring name, wstring surname, wstring patronymic, Address address) {
	this->id = id;
	this->name.assign(name);
	this->surname.assign(surname);
	this->patronymic.assign(patronymic);
	this->address = address;
}
Maid::Maid() {}
Maid::~Maid() {
}