#include <iostream>
#include "instance_representation/graph.hpp"

using namespace std;

int main() {
	Graph* G = Graph::CreateGraph(10);
	G->AddEdge(3, 5);
}
