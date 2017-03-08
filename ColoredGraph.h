#ifndef MI_PDP_CPP_COLOREDGRAPH_H
#define MI_PDP_CPP_COLOREDGRAPH_H


#include "Graph.h"
#include "Color.h"

class ColoredGraph : public Graph {

public:
    ColoredGraph(const Graph &graph);
    virtual ~ColoredGraph();
    bool hasColor(int vertex) const;
    Color getColor(int vertex) const;
    void setColor(int vertex, Color color);
    bool allColored() const;

private:
    Color *colors;
};


#endif
