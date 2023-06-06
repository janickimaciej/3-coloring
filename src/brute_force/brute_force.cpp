#include "brute_force.hpp"


bruteForce::bruteForce(Graph* graph)
{
    this->graph = graph;
    n = graph->getVertexCount();
}

bool bruteForce::Solve()
{
    return tryRec(0, Graph::copy(graph));
}

bool bruteForce::tryRec(int v, Graph* copy)
{
    if (v == n) return true;

    if (copy->getAvailableColors(v).size() > 0)
    {
        for (int color : copy->getAvailableColors(v))
        {
            Graph* g = Graph::copy(copy);
            g->setColor(v, color);
            if (tryRec(v + 1, g)) return true;
        }
        return false;
    }
    else return false;
}