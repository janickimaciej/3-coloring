#include "cycle_reduction.hpp"
#include <iostream>



CycleReduction::CycleReduction(Graph* graph)
{
	this->graph = graph;
	int n = graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	neighbours = new std::vector<int>[n];
	v = 0;
}
bool CycleReduction::hasCycle()
{
	return cycleRec(0, 0);
}

bool CycleReduction::cycleRec(int curr, int parent)
{
	parents[curr] = parent;
	visited[curr] = true;
	neighbours[curr] = (graph->getNeighbors(curr));
	for (int i = 0; i < neighbours[curr].size(); i++)
	{
		v = neighbours[curr][i];
		if (v == parent) continue;
		if (visited[v])
		{
			std::cout << "Start cycle " << curr << " end cycle " << v << "\n";
			deleteCycle(curr, v);
			return true;
		}
		if (cycleRec(v, curr)) return true;
	}
	return false;
}

void CycleReduction::deleteCycle(int start, int end)
{
	while (start != end)
	{
		std::cout << start << " ";
		start = parents[start];
	}
	std::cout << end << "\n";
}


CycleReduction::~CycleReduction()
{
	delete visited;
	delete [] neighbours;
	delete parents;
}
