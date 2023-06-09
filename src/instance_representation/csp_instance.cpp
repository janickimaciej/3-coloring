#include "csp_instance.hpp"
#include "inner_representation/inner_csp_instance.hpp"

CSPInstance* CSPInstance::create(int variables) {
	return new InnerRepresentation::InnerCSPInstance(variables, InnerRepresentation::InitialType::Variable);
}

CSPInstance* CSPInstance::copy(const CSPInstance* cspInstance) {
	return new InnerRepresentation::InnerCSPInstance(
		*dynamic_cast<const InnerRepresentation::InnerCSPInstance*>(cspInstance),
		InnerRepresentation::InitialType::Variable
	);
}

CSPInstance::~CSPInstance() { }
