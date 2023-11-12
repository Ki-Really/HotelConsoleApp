#include "Service.h"

Service::Service(int id,wstring service_name,int guest_id) {
	this->id = id;
	this->service_name.assign(service_name);
	this->guest_id = guest_id;
}
Service::Service() {};
Service::~Service(){}