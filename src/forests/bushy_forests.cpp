#include "bushy_forests.hpp"
#include <iostream>


BushyForest::BushyForest(std::vector<Instance*>* instances)
{
	this->instances = instances;
	setTarget(0);
}

void BushyForest::PrepareAll()
{
	for (int i = 0; i < instances->size(); i++)
	{
		setTarget(i);
		PlantForest();
		generateInstances();
	}
}

void BushyForest::setTarget(int target)
{
	if (inForest == nullptr)
	{
		delete[] inForest;
		delete[] neighbours;
		delete[] insideNode;
		forest.clear();
	}
	instance = instances->at(target);
	n = instance->graph->getVertexCount();
	inForest = new bool[n];
	neighbours = new std::vector<int>[n];
	insideNode = new bool[n];
	for (int i = 0; i < n; i++)
	{
		inForest[i] = false;
		insideNode[i] = false;
		neighbours[i] = instance->graph->getNeighbors(i);
	}
	outNeigh = 0;
}

void BushyForest::PlantForest()
{
	for (int i = 0; i < n; i++)
	{
		if (inForest[i]) continue;
		if (neighbours[i].size() < 4) continue;
		CheckSoil(i);
	}
}


void BushyForest::CheckSoil(int place)
{
	outNeigh = 0;
	for (int ver : neighbours[place])
	{
		if (!inForest[ver]) outNeigh++;
	}
	if (outNeigh >= 4) PlantTree(place);
}

void BushyForest::PlantTree(int root)
{
	inForest[root] = true;
	insideNode[root] = true;
	for (int ver : neighbours[root])
	{
		inForest[ver] = true;
	}
	for (int ver : neighbours[root])
	{
		CheckLeaf(ver);
	}
}

void BushyForest::CheckLeaf(int leaf)
{
	outNeigh = 0;
	for (int ver : neighbours[leaf])
	{
		if (!inForest[ver]) outNeigh++;
	}
	if (outNeigh >= 3) PlantTree(leaf);
}

void BushyForest::generateInstances()
{
	for (int i = n - 1; i >= 0; i--)
	{
		/*if (inForest[i])
		{
			if (insideNode[i]) instance->innerTree.push_back(instance->indexes[i]);
			instance->removeVertex(i);
		}*/
		if (insideNode[i])
		{
			instance->innerTree.push_back(instance->indexes[i]);
			//instance->removeVertex(i);
		}
	}
}