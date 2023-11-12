#pragma once
#include "Service.h"
#include "SqlConnection.h"
#include <vector>

class ServiceMapper {
public:
	SQLConnection* connection;
	ServiceMapper(SQLConnection* connection);
	~ServiceMapper();

	Service createService(Service& service,int guest_id);
	void deleteService(Service& service);
	void deleteServiceByGuestId(int id);
	std::vector<Service> getAll();
	//void updateSchedule(Service service);
	
};

