#include "csp_instance.hpp"
#include "inner_representation/inner_csp_instance.hpp"

CSPInstance* CSPInstance::create(int variables) {
	return new InnerCSPInstance(variables);
}

CSPInstance* CSPInstance::copy(CSPInstance* cspInstance) {
	return new InnerCSPInstance(*(InnerCSPInstance*)cspInstance);
}
