#include "graph.hpp"
#include "inner_csp_instance.hpp"

Graph* Graph::CreateGraph(int vertices) {
	return new InnerCSPInstance(vertices);
}
