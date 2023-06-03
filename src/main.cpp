#include <iostream>
#include "instance_representation/graph.hpp"

using namespace std;

int main() {
	Graph* G1 = Graph::create(4);
	G1->addEdge(0, 1);
	G1->addEdge(0, 2);
	G1->addEdge(0, 3);
	Graph* G2 = Graph::copy(G1);
	
	G2->removeVertex(1);
	vector<int> removedVertices;
	removedVertices.push_back(1);

	G2->setColor(0, 0);
	G2->setColor(1, 1);
	G2->setColor(2, 2);

	G1->copyColoring(G2, removedVertices);

	for(int i = 0; i < 4; i++) { // print available colors for each vertex
		vector<int> colors = G1->getAvailableColors(i);
		cout << i << ": ";
		for(int j = 0; j < colors.size(); j++) {
			cout << colors[j] << " ";
		}
		cout << endl;
	}
}
