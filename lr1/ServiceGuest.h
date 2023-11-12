#pragma once
#include <string>
#include "Guest.h"
class ServiceGuest {
public:
	int service_id;
	int guest_id;

	ServiceGuest(int service_id, int guest_id);
	ServiceGuest();
	~ServiceGuest();
};