#pragma once
#include "SQLConnection.h"
#include "Address.h"

class AddressMapper {
public:
	SQLConnection* connection;
	AddressMapper(SQLConnection* connection);
	~AddressMapper();
	Address createAddress(Address& address);
	void deleteAddress(Address& address);
	void updateAddress(Address& address);
};