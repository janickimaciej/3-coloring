#include <iostream>
#include "instance_representation/graph.hpp"
#include "graph6/G6Parser.hpp"

using namespace std;

int main() {
	Graph* G = Graph::createGraph(5);
	G->addEdge(2, 4);
	G->addEdge(2, 1);
	std::vector<int> neighbors = G->getNeighbors(2);
	for(int i = 0; i < neighbors.size(); i++) {
		cout << neighbors[i] << endl;
	}
}
