#ifndef MI_PDP_CPP_BIPARTITE_H
#define MI_PDP_CPP_BIPARTITE_H

#include "Color.h"
#include "Graph.h"

class Bipartite {
private:
    Graph &graph;
    Color *colors;

    void dfs(int node, Color color);
    Color otherColor(Color color);
    bool hasColor(int vertex) const;
    Color getColor(int vertex) const;
    void setColor(int vertex, Color color);
public:
    Bipartite(Graph & _graph);

    ~Bipartite();
    bool isBipartite;
};


#endif
