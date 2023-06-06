#include "brute_force.hpp"


BruteForce::BruteForce(Graph* graph)
{
    this->graph = graph;
    n = graph->getVertexCount();
}

bool BruteForce::Solve()
{
    return tryRec(0, Graph::copy(graph));
}

bool BruteForce::tryRec(int v, Graph* copy)
{
    if (v == n)
    {
        graph = copy;
        return true;
    }

    if (copy->getAvailableColors(v).size() > 0)
    {
        /*std::vector<int> colors = copy->getAvailableColors(0);
        std::vector<int> colors2 = copy->getAvailableColors(1);
        std::vector<int> colors3 = copy->getAvailableColors(2);
        int a = 5;*/
        for (int color : copy->getAvailableColors(v))
        {
            Graph* g = Graph::copy(copy);
            giveColor(g, v, color);
            if (tryRec(v + 1, g)) return true;
        }
        return false;
    }
    else return false;
}

void BruteForce::giveColor(Graph *g, int v, int color)
{
    g->setColor(v, color);
    for (int ver : g->getNeighbors(v))
    {
        if (g->isColorAvailable(ver, color))
        {
            g->disableColor(ver, color);
        }
    }
}
