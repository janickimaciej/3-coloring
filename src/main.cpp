#include <iostream>
#include "instance_representation/graph.hpp"

using namespace std;

int main() {
	Graph* G = Graph::CreateGraph(10);
	cout << G->hasEdge(3, 5) << endl;
	G->AddEdge(3, 5);
	cout << G->hasEdge(3, 5) << endl;
	G->removeEdge(3, 5);
	cout << G->hasEdge(3, 5) << endl;
}
