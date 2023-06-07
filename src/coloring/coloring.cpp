#include "coloring.hpp"


Coloring::Coloring(Graph* graph)
{
	this->graph = graph;
	this->instances = nullptr;
	this->instance = nullptr;
	n = 0;
	solved = false;
}

bool Coloring::Solve()
{
	InitialReduction initial(Graph::copy(graph));
	instances = initial.Reduce();
	BushyForest forest(instances);
	forest.PrepareAll();
	Rule* rule = instances->at(0)->rules[2];
	Result result;
	for (int i = 0; i < instances->size(); i++)
	{
		result = CSPSolver::solve(dynamic_cast<CSPInstance*>(instances->at(i)->graph));
		if (result == Result::Success)
		{
			instance = instances->at(i);
			solved = true;
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
	return colorForest(0, Graph::copy(graph));
}

bool Coloring::colorForest(int v, Graph* copy)
{
	if (v == instance->innerTree.size())
	{
		Instance* cp = Instance::copy(instance);
		return (checkRest(copy, Instance::copy(instance)));
	}
	else
	{
		Graph* g;
		int ver = instance->innerTree[v];
		if (ver < n)
		{
			for (int color : copy->getAvailableColors(ver))
			{
				g = Graph::copy(copy);
				Instance::giveColor(g, ver, color);
				if (colorForest(v + 1, g)) return true;
			}
		}
		else
		{
			std::vector<int> unMerged = *instance->unMerge(ver, n);
			for (int color : copy->getAvailableColors(unMerged[0]))
			{
				g = Graph::copy(copy);
				for (int mer : unMerged)
				{
					Instance::giveColor(g, mer, color);
				}
				if (colorForest(v + 1, g)) return true;
			}
		}
		return false;
		
	}
}

void Coloring::copyColoring()
{
	for (int i = 0; i < instance->graph->getVertexCount(); i++)
	{
		int color = instance->graph->getAvailableColors(i).at(0);
		if (instance->indexes[i] < n)
		{
			Instance::giveColor(graph, instance->indexes[i], color);
		}
		else
		{
			std::vector<int> unmerged = *instance->unMerge(instance->indexes[i],n);
			for (int ver : unmerged)
			{
				Instance::giveColor(graph, ver, color);
			}
		}
		
	}
}


bool Coloring::checkRest(Graph* copy, Instance* copyInst)
{
	int ver;
	for (int i = copyInst->deleted.size() - 1; i >= 0;  i--)
	{
		ver = copyInst->deleted[i];
		if (ver < n)
		{
			Rule* rul = copyInst->rules[ver];
			Rule* rule = instances->at(0)->rules[2];
			copyInst->rules[ver]->apply(copyInst, graph);
			if (copyInst->rules[ver]->apply(copyInst, graph))
			{
				continue;
			}
			if (copy->getAvailableColors(ver).size() > 0)
			{
				Instance::giveColor(copy, ver, copy->getAvailableColors(ver).at(0));
			}
			else return false;
		}
		else
		{
			std::vector<int> unMerged = *copyInst->unMerge(ver, n);
			for (int mer : unMerged)
			{
				if (copyInst->rules[ver]->apply(copyInst, graph))
				{
					continue;
				}
				if (copy->getAvailableColors(mer).size() > 0)
				{
					Instance::giveColor(copy, mer, copy->getAvailableColors(mer).at(0));
				}
				else return false;
			}
		}
	}
	graph = copy;
	return true;
}