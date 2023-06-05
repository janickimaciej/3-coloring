#include "graph.hpp"
#include "inner_representation/inner_csp_instance.hpp"

Graph* Graph::create(int vertices) {
	return new InnerRepresentation::InnerCSPInstance(vertices);
}

Graph* Graph::copy(const Graph* graph) {
	return new InnerRepresentation::InnerCSPInstance(
		*dynamic_cast<const InnerRepresentation::InnerCSPInstance*>(graph)
	);
}

Graph::~Graph() { }
