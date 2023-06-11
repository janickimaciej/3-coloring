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
		if (n == 28)
		{
			std::vector<int> neigh = instance->graph->getNeighbors(i);
			int b = 7;
		}
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
		if (n == 28)
		{
			std::vector<int> neigh = instance->graph->getNeighbors(i);
			int b = 7;
		}
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
	int b = 3;
	int i = 0;
	if (instance->graph->getVertexCount() == 0) return false;
	while (1)
	{
		// do zmiany ?
		if (threes[i] && !visited[i])
		{
			if (cycleRec(i, i))
			{
				Update();
				//hasReduced = true;
				//continue;
				return true;
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
	int t = 5;


	int w1, w2, w3;
	
	// Option: cycle is even
	if (cycle.size() % 2 == 0)
	{
		std::vector<int> ruleCycle;
		std::vector<int> ruleNeigh;
		for (int ver : cycle)
		{
			ruleCycle.push_back(instance->indexes[ver]);
			ruleNeigh.push_back(instance->indexes[findNeighbour(ver)]);
		}
		EvenRule* even = new EvenRule(ruleCycle, ruleNeigh);
		for (int ver : cycle)
		{
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
			addNeighbourCycle(instance, w1, i % cycle.size());
			
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
		newInst->addEdge(w1, w2);
		addNeighbourCycle(newInst, w1, 1);
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
			instance->addEdge(n, ver);
		}
		for (int ver : neighbours[w2])
		{
			if (!instance->graph->hasEdge(n,ver)) instance->addEdge(n, ver);
		}
		for (int ver : neighbours[cycle[2]])
		{
			if (!instance->graph->hasEdge(n, ver)) instance->addEdge(n, ver);
		}
		int neigh = findNeighbour(cycle[2]);
		std::vector<int> trueCycle;
		for (int ver : cycle)
		{
			trueCycle.push_back(instance->indexes[ver]);
		}
		trueCycle.push_back(instance->indexes[w1]);
		trueCycle.push_back(instance->indexes[w2]);
		instance->addEdge(n, findNeighbour(cycle[2]));
		toDeletion[w1] = true;
		toDeletion[w2] = true;
		Clear();
		return;
	}

	// Option: cycle length > 3 and odd
	
	w1 = findNeighbour(cycle[0]);
	w2 = findNeighbour(cycle[1]);

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
		newInst->addEdge(n, ver);
	}
	for (int ver : neighbours[w2])
	{
		if (!newInst->graph->hasEdge(n,ver)) newInst->addEdge(n, ver);
	}
	newInst->addEdge(n, findNeighbour(cycle[2]));
	toDeletion[w1] = true;
	toDeletion[w2] = true;
	addNeighbourCycle(newInst, n, 2);
	Clear(newInst);
	toDeletion[w1] = false;
	toDeletion[w2] = false;

	// merge three neighbours

	w3 = findNeighbour(cycle[2]);
	if (!instance->graph->hasEdge(w1, w3))
	{
		newInst = Instance::copy(instance);
		instances->push_back(newInst);
		newInst->addVertex();
		std::vector<int> merge2;
		merge2.push_back(newInst->indexes[n]);
		merge2.push_back(newInst->indexes[w1]);
		merge2.push_back(newInst->indexes[w2]);
		merge2.push_back(newInst->indexes[w3]);
		newInst->superVertices.push_back(merge2);
		for (int ver : neighbours[w1])
		{
			newInst->addEdge(n, ver);
		}
		for (int ver : neighbours[w2])
		{
			if (!newInst->graph->hasEdge(n, ver)) newInst->addEdge(n, ver);
		}
		for (int ver : neighbours[w3])
		{
			if (!newInst->graph->hasEdge(n, ver)) newInst->addEdge(n, ver);
		}
		newInst->addVertex();
		std::vector<int> merge3;
		merge3.push_back(newInst->indexes[n + 1]);
		merge3.push_back(newInst->indexes[cycle[0]]);
		merge3.push_back(newInst->indexes[cycle[2]]);
		newInst->superVertices.push_back(merge3);
		newInst->addEdge(n, n + 1);
		newInst->addEdge(n + 1, cycle[3]);
		newInst->addEdge(n + 1, cycle[cycle.size() - 1]);


		for (int i = 3; i < cycle.size(); i++)
		{
			toDeletion[cycle[i]] = false;
		}
		toDeletion[w1] = true;
		toDeletion[w2] = true;
		toDeletion[w3] = true;

		Clear(newInst);

		toDeletion[w1] = false;
		toDeletion[w2] = false;
		toDeletion[w3] = false;
		for (int i = 3; i < cycle.size(); i++)
		{
			toDeletion[cycle[i]] = true;
		}
	}

	// add edge to two neighbours
	instance->addEdge(w1, w2);
	addNeighbourCycle(instance, w1, 1);
	Clear();
}

void CycleReduction::Clear()
{
	int b = 7;
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
		if (toDeletion[i])
		{
			inst->removeVertex(i);
		}
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
			int d = 6;
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

void CycleReduction::addNeighbourCycle(Instance* inst, int w1, int target)
{
	std::vector<int> ruleCycle;
	for (int ver : cycle) ruleCycle.push_back(inst->indexes[ver]);
	NeighbourRule* neighbour = new NeighbourRule(inst->indexes[w1], target, ruleCycle);
	for (int ver : ruleCycle)
	{
		if (ver < inst->originalN) inst->rules[ver] = (Rule*)neighbour;
		else
		{
			std::vector<int> unMerged = *inst->unMerge(ver, n);
			for (int mer : unMerged)
			{
				inst->rules[mer] = (Rule*)neighbour;
			}
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
		if (n == 25)
		{
			std::vector<int> neigh = instance->graph->getNeighbors(i);
			int n = 0;
		}
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

