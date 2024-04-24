#pragma once
#include "Service.h"
#include "SqlConnection.h"
#include <vector>
#include"ServiceGuest.h"

class ServiceGuestMapper {
public:
	SQLConnection* connection;
	ServiceGuestMapper(SQLConnection* connection);
	~ServiceGuestMapper();

	void createServiceGuest(ServiceGuest serviceGuest);
	void deleteServiceGuest(int service_id);
	std::vector<ServiceGuest> getAll();

};

#pragma once
