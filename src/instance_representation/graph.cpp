#include "graph.hpp"
#include "inner_representation/inner_csp_instance.hpp"

Graph* Graph::create(int vertices) {
	return new InnerCSPInstance(vertices);
}

Graph* Graph::copy(Graph* graph) {
	return new InnerCSPInstance(*(InnerCSPInstance*)graph);
}
