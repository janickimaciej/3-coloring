#include "graph.hpp"
#include "inner_csp_instance.hpp"

Graph* Graph::createGraph(int vertices) {
	return new InnerCSPInstance(vertices);
}

Graph* Graph::copy(Graph* graph) {
	return new InnerCSPInstance(*(InnerCSPInstance*)graph);
}
