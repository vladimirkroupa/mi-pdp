#ifndef MI_PDP_CPP_BIPARTITE_H
#define MI_PDP_CPP_BIPARTITE_H

#include "Color.h"
#include "Graph.h"
#include "ColoredGraph.h"

class Bipartite {
private:
    ColoredGraph * graph;

    void dfs(int node, Color color);
    Color otherColor(Color color);
public:
    Bipartite(const Graph& graph);
    ~Bipartite();
    bool isBipartite;
};


#endif
