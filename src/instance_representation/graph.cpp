#include "graph.hpp"
#include "inner_representation/inner_csp_instance.hpp"

Graph* Graph::create(int vertices) {
	return new InnerRepresentation::InnerCSPInstance(vertices);
}

Graph* Graph::copy(const Graph* graph) {
	return new InnerRepresentation::InnerCSPInstance(*(InnerRepresentation::InnerCSPInstance*)graph);
}
