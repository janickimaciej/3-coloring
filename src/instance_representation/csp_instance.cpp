#include "csp_instance.hpp"
#include "inner_representation/inner_csp_instance.hpp"

CSPInstance* CSPInstance::create(int variables) {
	return new InnerRepresentation::InnerCSPInstance(variables);
}

CSPInstance* CSPInstance::copy(const CSPInstance* cspInstance) {
	return new InnerRepresentation::InnerCSPInstance(*(InnerRepresentation::InnerCSPInstance*)cspInstance);
}

CSPInstance::~CSPInstance() { }
