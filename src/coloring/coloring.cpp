#include "coloring.hpp"


Coloring::Coloring(Graph* graph)
{
	this->graph = graph;
	this->instances = nullptr;
}

bool Coloring::Solve()
{
	InitialReduction initial(Graph::copy(graph));
	instances = initial.Reduce();
	BushyForest forest(instances);
	forest.PrepareAll();
	Result result;
	for (int i = 0; i < instances->size(); i++)
	{
		result = CSPSolver::solve(dynamic_cast<CSPInstance*>(instances->at(i)->graph));
		if (result == Result::Success)
		{
			if (tryToColor(i)) return true;
		}
	}
	return false;
}

bool Coloring::tryToColor(int inst)
{
	n = graph->getVertexCount();
	instance = instances->at(inst);
	copyColoring();
	colorForest(0, Graph::copy(graph));
}

bool Coloring::colorForest(int v, Graph* copy)
{
	if (v == instance->innerTree.size())
	{
		return (checkRest(copy));
	}
	else
	{
		Graph* g;
		int ver = instance->indexes[instance->innerTree[v]];
		if (ver < n)
		{
			for (int color : copy->getAvailableColors(ver))
			{
				g = Graph::copy(copy);
				g->setColor(ver, color);
				if (colorForest(v + 1, g)) return true;
			}
		}
		else
		{
			std::vector<int> unMerged;
			unMerge(ver, &unMerged);
			for (int color : copy->getAvailableColors(unMerged[0]))
			{
				g = Graph::copy(copy);
				for (int mer : unMerged)
				{
					g->setColor(mer, color);
				}
				if (colorForest(v + 1, g)) return true;
			}
		}
		
	}
}

void Coloring::copyColoring()
{
	for (int i = 0; i < instance->graph->getVertexCount(); i++)
	{
		int color = instance->graph->getAvailableColors(i).at(0);
		if (instance->indexes[i] < n)
		{
			graph->setColor(instance->indexes[i], color);
		}
		else
		{
			std::vector<int> unmerged;
			unMerge(instance->indexes[i], &unmerged);
			for (int ver : unmerged)
			{
				graph->setColor(ver, color);
			}
		}
		
	}
}

void Coloring::unMerge(int v, std::vector<int>* unmerged)
{
	for (std::vector<int> merge : instance->superVertices)
	{
		if (merge[0] == v)
		{
			for (int i = 1; i < merge.size(); i++)
			{
				if (merge[i] < n)
				{
					unmerged->push_back(merge[i]);
				}
				else
				{
					unMerge(merge[i], unmerged);
				}
			}
		}
	}
}

bool Coloring::checkRest(Graph* copy)
{
	int ver;
	for (int i = instance->deleted.size() - 1; i >= 0;  i--)
	{
		ver = instance->deleted[i];
		if (ver < n)
		{
			if (copy->getAvailableColors(ver).size() > 0)
			{
				copy->setColor(ver, copy->getAvailableColors(ver).at(0));
			}
			else return false;
		}
		else
		{
			std::vector<int> unMerged;
			unMerge(ver, &unMerged);
			for (int mer : unMerged)
			{
				if (copy->getAvailableColors(mer).size() > 0)
				{
					copy->setColor(mer, copy->getAvailableColors(mer).at(0));
				}
				else return false;
			}
		}
	}
	graph = copy;
	return true;
}
