#include "Address.h"
using namespace std;

Address::Address(int id,wstring _country,wstring _city,wstring _street,int building) {
	this->id = id;
	country.assign(_country);
	city.assign(_city);
	street.assign(_street);
	this->building = building;
}
Address::Address(){}
Address::~Address() {}