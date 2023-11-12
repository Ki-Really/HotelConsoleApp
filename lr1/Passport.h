#pragma once
#include <string>
class Passport {
public:
	int id;
	int number;
	std::wstring issuance;
	std::wstring givenBy;

	Passport(int id, int number,std::wstring _issuance, std::wstring _givenBy);
	Passport();
	~Passport();
};