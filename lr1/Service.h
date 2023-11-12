#pragma once
#include <string>
#include "Guest.h"
class Service {
public:
	int id;
	wstring service_name;
	int guest_id;
	
	
	Service(int id,wstring service_name,int guest_id);
	Service();
	~Service();
};