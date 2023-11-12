#pragma once
#include <string>
using namespace std;

class Address {
public:
	int id;
	std::wstring country;
	wstring city;
	wstring street;
	int building;
	Address(int id, wstring _country, wstring _city, wstring _street, int building);
	Address();
	~Address();
	
};
