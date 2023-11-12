#include "ServiceGuest.h"

ServiceGuest::ServiceGuest(int service_id, int guest_id) {
	this->service_id = service_id;
	this->guest_id = guest_id;
}
ServiceGuest::ServiceGuest() {};
ServiceGuest::~ServiceGuest() {}