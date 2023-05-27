#include <iostream>
#include "instance_representation/graph.hpp"
#include "graph6/G6Parser.hpp"

using namespace std;

int main() {
	Graph* G1 = Graph::createGraph(5);
	G1->addEdge(2, 4);
	G1->addEdge(2, 1);
	Graph* G2 = Graph::copy(G1);
	std::vector<int> neighbors = G2->getNeighbors(2);
	for(int i = 0; i < neighbors.size(); i++) {
		cout << neighbors[i] << endl;
	}
}
