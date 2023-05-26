#include "graph.hpp"
#include "inner_csp_instance.hpp"

Graph* Graph::createGraph(int vertices) {
	return new InnerCSPInstance(vertices);
}
