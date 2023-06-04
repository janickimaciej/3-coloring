#include "cycle_reduction.hpp"
#include <iostream>
#include "../graph6/G6Parser.hpp"



CycleReduction::CycleReduction(std::vector<Graph*>* instances)
{
	this->instances = instances;
	setTarget(0);
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
			deleteCycle(curr, v);
			return true;
		}
	}
	for (int i = 0; i < neighbours[curr].size(); i++)
	{
		v = neighbours[curr][i];
		if (!threes[v]) continue;
		if (v == parent) continue;
		if (cycleRec(v, curr)) return true;
	}
	return false;
}

void CycleReduction::deleteCycle(int start, int end)
{
	while (start != end)
	{
		toDeletion[start] = true;
		cycle.push_back(start);
		start = parents[start];
	}
	toDeletion[end] = true;
	cycle.push_back(end);


	int w1, w2, w3;
	
	// Option: cycle is even
	if (cycle.size() % 2 == 0)
	{
		// Delete All
		Clear();
		return;
	}

	// Option: two neighbours of the cycle are connected
	w1 = findNeighbour(cycle[0]);
	for (int i = 1; i <= cycle.size(); i++)
	{
		w2 = findNeighbour(cycle[i % cycle.size()]);
		if (graph->hasEdge(w1, w2))
		{
			// Delete All
			Clear();
			return;
		}
		w1 = w2;
	}

	Graph* newInst;

	// Option: cycle length three
	if (cycle.size() == 3)
	{

		// link two neighbours
		newInst = Graph::copy(graph);
		instances->push_back(newInst);
		w1 = findNeighbour(cycle[0]);
		w2 = findNeighbour(cycle[1]);
		newInst->addEdge(w1, w2);
		Clear(newInst);

		// merge neighbours
		graph->addVertex();
		for (int ver : neighbours[w1])
		{
			graph->addEdge(n, ver);
		}
		for (int ver : neighbours[w2])
		{
			graph->addEdge(n, ver);
		}
		graph->addEdge(n, findNeighbour(cycle[2]));
		toDeletion[w1] = true;
		toDeletion[w2] = true;
		Clear();
		return;
	}

	// Option: cycle length > 3 and odd
	
	// add edge to two neighbours
	newInst = Graph::copy(graph);
	instances->push_back(newInst);
	w1 = findNeighbour(cycle[0]);
	w2 = findNeighbour(cycle[1]);
	newInst->addEdge(w1, w2);
	Clear(newInst);

	// merge two neighbours

	newInst = Graph::copy(graph);
	instances->push_back(newInst);
	newInst->addVertex();
	for (int ver : neighbours[w1])
	{
		newInst->addEdge(n, ver);
	}
	for (int ver : neighbours[w2])
	{
		newInst->addEdge(n, ver);
	}
	newInst->addEdge(n, findNeighbour(cycle[2]));
	toDeletion[w1] = true;
	toDeletion[w2] = true;
	Clear(newInst);

	// merge three neighbours
	instances->push_back(Graph::copy(graph));
	w3 = findNeighbour(cycle[2]);
	graph->addVertex();
	for (int ver : neighbours[w1])
	{
		graph->addEdge(n, ver);
	}
	for (int ver : neighbours[w2])
	{
		graph->addEdge(n, ver);
	}
	for (int ver : neighbours[w3])
	{
		graph->addEdge(n, ver);
	}
	graph->addVertex();
	graph->addEdge(n, n + 1);
	graph->addEdge(n + 1, cycle[3]);
	graph->addEdge(n + 1, cycle[cycle.size() - 1]);


	for (int i = 3; i < cycle.size(); i++)
	{
		toDeletion[cycle[i]] = false;
	}
	toDeletion[w1] = true;
	toDeletion[w2] = true;
	toDeletion[w3] = true;

	Clear();

}

void CycleReduction::Clear()
{
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i]) graph->removeVertex(i);
	}
}

void CycleReduction::Clear(Graph* g)
{
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i]) g->removeVertex(i);
	}
}

int CycleReduction::findNeighbour(int cycleVer)
{
	for (int ver : neighbours[cycleVer])
	{
		if (!toDeletion[ver])
		{
			return ver;
		}
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
	/*for (int i = 0; i < n; i++)
	{
		std::cout << i << ": ";
		for (int ver : neighbours[i])
		{
			std::cout << ver << " ";
		}
		std::cout << "\n";
	}*/
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

