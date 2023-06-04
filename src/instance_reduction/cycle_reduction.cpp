#include "cycle_reduction.hpp"
#include <iostream>



CycleReduction::CycleReduction(std::vector<Graph*>* instances)
{
	this->instances = instances;
	setTarget(0);
	this->instances->push_back(instances->at(0));
}

void CycleReduction::setTarget(int g)
{
	this->graph = instances->at(g);
	n = graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	toDeletion = new bool[n];
	threes = new bool[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		toDeletion[i] = false;
		neighbours[i] = graph->getNeighbors(i);
		if (neighbours[i].size() == 3) threes[i] = true;
		else threes[i] = false;
	}
	v = 0;
	hasReduced = false;
}

bool CycleReduction::Reduce()
{
	hasReduced = false;
	while (1)
	{
		for (int i = 0; i < n; i++)
		{
			if (threes[i] && !visited[i])
			{
				if (cycleRec(i, i))
				{
					Update();
					hasReduced = true;
					continue;
				}
			}
		}
		break;
	}
	return hasReduced;
}

bool CycleReduction::cycleRec(int curr, int parent)
{
	parents[curr] = parent;
	visited[curr] = true;
	for (int i = 0; i < neighbours[curr].size(); i++)
	{
		v = neighbours[curr][i];
		if (!threes[v]) continue;
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
		toDeletion[start] = true;
		cycle.push_back(start);
		start = parents[start];
	}
	toDeletion[end] = true;
	cycle.push_back(end);
	std::cout << end << "\n";

	// Option: cycle is even
	if (cycle.size() % 2 == 0)
	{
		// Delete All
		return;
	}

	// Option: two neighbours of the cycle are connected
	for (int i = 0; i < cycle.size(); i++)
	{
		for (int ver : neighbours[cycle[i]])
		{
			if (graph->hasEdge(ver, cycle[(i + 1) % cycle.size()]))
			{
				// Delete All
				return;
			}
		}
	}


}

void CycleReduction::Clear()
{
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i]) graph->removeVertex(i);
	}
}

void CycleReduction::Update()
{
	delete[] visited;
	delete[] neighbours;
	delete[] parents;
	delete[] threes;
	delete[] toDeletion;
	cycle.clear();

	n = graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	toDeletion = new bool[n];
	threes = new bool[n];
	neighbours = new std::vector<int>[n];

	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		toDeletion[i] = false;
		neighbours[i] = graph->getNeighbors(i);
		if (neighbours[i].size() == 3) threes[i] = true;
		else threes[i] = false;
	}
	for (int i = 0; i < n; i++)
	{
		std::cout << i << ": ";
		for (int ver : neighbours[i])
		{
			std::cout << ver << " ";
		}
		std::cout << "\n";
	}
	v = 0;
}


CycleReduction::~CycleReduction()
{
	delete[] visited;
	delete[] neighbours;
	delete[] parents;
	delete[] threes;
	delete[] toDeletion;
}

