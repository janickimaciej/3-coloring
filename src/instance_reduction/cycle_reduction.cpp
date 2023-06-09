#include "cycle_reduction.hpp"
#include <iostream>
#include "../graph6/G6Parser.hpp"
#include "../instance_representation/rules/even_rule.hpp"
#include "../instance_representation/rules/neighbour_rule.hpp"



CycleReduction::CycleReduction(std::vector<Instance*>* instances)
{
	this->instances = instances;
	this->instance = instances->at(0);
	n = instance->graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	toDeletion = new bool[n];
	threes = new bool[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		toDeletion[i] = false;
		neighbours[i] = instance->graph->getNeighbors(i);
		if (neighbours[i].size() == 3) threes[i] = true;
		else threes[i] = false;
	}
	v = 0;
	hasReduced = false;
}


void CycleReduction::setTarget(int g)
{
	delete[] visited;
	delete[] neighbours;
	delete[] parents;
	delete[] threes;
	delete[] toDeletion;
	cycle.clear();
	this->instance = instances->at(g);
	n = instance->graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	toDeletion = new bool[n];
	threes = new bool[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		toDeletion[i] = false;
		neighbours[i] = instance->graph->getNeighbors(i);
		if (neighbours[i].size() == 3) threes[i] = true;
		else threes[i] = false;
	}
	v = 0;
	hasReduced = false;
}

bool CycleReduction::Reduce()
{
	hasReduced = false;
	Rule* rule = instances->at(0)->rules[0];
	int b = 3;
	int i = 0;
	while (1)
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
		i++;
		if (i == instance->graph->getVertexCount()) break;
	}
	return hasReduced;
}

bool CycleReduction::cycleRec(int curr, int parent)
{
	parents[curr] = parent;
	visited[curr] = true;
	int min = -1;
	int minV;
	for (int i = 0; i < neighbours[curr].size(); i++)
	{
		int v = neighbours[curr][i];
		if (!threes[v]) continue;
		if (v == parent) continue;
		if (visited[v])
		{
			if (min == -1)
			{
				min = cycleLength(curr, v);
				minV = v;
			}
			else if (cycleLength(curr, v) < min)
			{
				min = cycleLength(curr, v);
				minV = v;
			}
		}
	}
	if (min != -1)
	{
		deleteCycle(curr, minV);
		return true;
	}
	for (int i = 0; i < neighbours[curr].size(); i++)
	{
		int v = neighbours[curr][i];
		if (!threes[v]) continue;
		if (v == parent) continue;
		Rule* rule = instances->at(0)->rules[0];
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
		std::vector<int> ruleCycle;
		std::vector<int> ruleNeigh;
		for (int ver : cycle)
		{
			if (instance->indexes[ver] == 68)
			{
				int c = 4;
			}
			ruleCycle.push_back(instance->indexes[ver]);
			ruleNeigh.push_back(instance->indexes[findNeighbour(ver)]);
		}
		EvenRule* even = new EvenRule(ruleCycle, ruleNeigh);
		for (int ver : cycle)
		{
			Rule* rule = instance->rules[172];
			int verI = instance->indexes[ver];
			if (verI < instance->originalN) instance->rules[verI] = (Rule*) even;
			else
			{
				std::vector<int> unMerged = *instance->unMerge(verI, n);
				for (int mer : unMerged)
				{
					instance->rules[mer] = (Rule*) even;
				}
			}
		}
		Rule* rule = instance->rules[172];
		// Delete All
		Clear();
		return;
	}

	// Option: two neighbours of the cycle are connected
	w1 = findNeighbour(cycle[0]);
	for (int i = 1; i <= cycle.size(); i++)
	{
		w2 = findNeighbour(cycle[i % cycle.size()]);
		if (instance->graph->hasEdge(w1, w2))
		{
			std::vector<int> ruleCycle;
			for (int ver : cycle) ruleCycle.push_back(instance->indexes[ver]);
			NeighbourRule* neighbour = new NeighbourRule(instance->indexes[w1], i % cycle.size(), ruleCycle);
			for (int ver : cycle)
			{
				if (ver < instance->originalN) instance->rules[ver] = (Rule*)neighbour;
				else
				{
					std::vector<int> unMerged = *instance->unMerge(ver,n);
					for (int mer : unMerged)
					{
						instance->rules[mer] = (Rule*)neighbour;
					}
				}
			}
			
			// Delete All
			Clear();
			return;
		}
		w1 = w2;
	}

	Instance* newInst;

	// Option: cycle length three
	if (cycle.size() == 3)
	{

		// link two neighbours
		newInst = Instance::copy(instance);
		instances->push_back(newInst);
		w1 = findNeighbour(cycle[0]);
		w2 = findNeighbour(cycle[1]);
		newInst->graph->addEdge(w1, w2);
		Clear(newInst);

		// merge neighbours
		instance->addVertex();
		std::vector<int> merge;
		merge.push_back(instance->indexes[n]);
		merge.push_back(instance->indexes[cycle[2]]);
		merge.push_back(instance->indexes[w1]);
		merge.push_back(instance->indexes[w2]);
		instance->superVertices.push_back(merge);
		for (int ver : neighbours[w1])
		{
			instance->graph->addEdge(n, ver);
		}
		for (int ver : neighbours[w2])
		{
			if (!instance->graph->hasEdge(n,ver)) instance->graph->addEdge(n, ver);
		}
		for (int ver : neighbours[cycle[2]])
		{
			if (!instance->graph->hasEdge(n, ver)) instance->graph->addEdge(n, ver);
		}
		instance->graph->addEdge(n, findNeighbour(cycle[2]));
		toDeletion[w1] = true;
		toDeletion[w2] = true;
		Clear();
		return;
	}

	// Option: cycle length > 3 and odd
	
	// add edge to two neighbours
	newInst = Instance::copy(instance);
	instances->push_back(newInst);
	w1 = findNeighbour(cycle[0]);
	w2 = findNeighbour(cycle[1]);
	newInst->graph->addEdge(w1, w2);
	Clear(newInst);

	// merge two neighbours

	newInst = Instance::copy(instance);
	instances->push_back(newInst);
	newInst->addVertex();
	std::vector<int> merge;
	merge.push_back(newInst->indexes[n]);
	merge.push_back(newInst->indexes[w1]);
	merge.push_back(newInst->indexes[w2]);
	newInst->superVertices.push_back(merge);
	for (int ver : neighbours[w1])
	{
		newInst->graph->addEdge(n, ver);
	}
	for (int ver : neighbours[w2])
	{
		if (!newInst->graph->hasEdge(n,ver)) newInst->graph->addEdge(n, ver);
	}
	newInst->graph->addEdge(n, findNeighbour(cycle[2]));
	toDeletion[w1] = true;
	toDeletion[w2] = true;
	Clear(newInst);

	// merge three neighbours
	w3 = findNeighbour(cycle[2]);
	instance->addVertex();
	std::vector<int> merge2;
	merge2.push_back(instance->indexes[n]);
	merge2.push_back(instance->indexes[w1]);
	merge2.push_back(instance->indexes[w2]);
	merge2.push_back(instance->indexes[w3]);
	instance->superVertices.push_back(merge2);
	for (int ver : neighbours[w1])
	{
		instance->graph->addEdge(n, ver);
	}
	for (int ver : neighbours[w2])
	{
		if (!instance->graph->hasEdge(n,ver)) instance->graph->addEdge(n, ver);
	}
	for (int ver : neighbours[w3])
	{
		if (!instance->graph->hasEdge(n,ver)) instance->graph->addEdge(n, ver);
	}
	instance->addVertex();
	std::vector<int> merge3;
	merge3.push_back(instance->indexes[n + 1]);
	merge3.push_back(instance->indexes[cycle[0]]);
	merge3.push_back(instance->indexes[cycle[2]]);
	instance->superVertices.push_back(merge3);
	instance->graph->addEdge(n, n + 1);
	instance->graph->addEdge(n + 1, cycle[3]);
	instance->graph->addEdge(n + 1, cycle[cycle.size() - 1]);


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
		if (toDeletion[i])
		{
			instance->removeVertex(i);
		}
	}
}

void CycleReduction::Clear(Instance* inst)
{
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i]) inst->removeVertex(i);
	}
}

int CycleReduction::findNeighbour(int cycleVer)
{
	std::vector<int> ne = neighbours[cycleVer];
	int b = 56;
	for (int ver : neighbours[cycleVer])
	{
		if (!toDeletion[ver])
		{
			return ver;
		}
	}
}

int CycleReduction::cycleLength(int start, int end)
{
	int length = 0;
	while (start != end)
	{
		length++;
		start = parents[start];
	}
	return length;
}


void CycleReduction::Update()
{
	delete[] visited;
	delete[] neighbours;
	delete[] parents;
	delete[] threes;
	delete[] toDeletion;
	cycle.clear();

	n = instance->graph->getVertexCount();
	visited = new bool[n];
	parents = new int[n];
	toDeletion = new bool[n];
	threes = new bool[n];
	neighbours = new std::vector<int>[n];

	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		toDeletion[i] = false;
		neighbours[i] = instance->graph->getNeighbors(i);
		if (neighbours[i].size() == 3) threes[i] = true;
		else threes[i] = false;
	}
	v = 0;
}


CycleReduction::~CycleReduction()
{
	if (n != -1)
	{
		delete[] visited;
		delete[] neighbours;
		delete[] parents;
		delete[] threes;
		delete[] toDeletion;
	}
}

