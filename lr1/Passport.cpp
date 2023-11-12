#include "Passport.h"


Passport::Passport(int id, int number, std::wstring _issuance, std::wstring _givenBy) {
	this->id = id;
	this->number = number;
	
	this->issuance.assign(_issuance);
	this->givenBy.assign(_givenBy);
}
Passport::Passport(){}
Passport::~Passport() {}