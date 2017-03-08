#include "ColoredGraph.h"

ColoredGraph::ColoredGraph(const Graph &graph) : Graph(graph) {
    colors = new Color[this->getVertexCount()];
    for (int i = 0; i < getVertexCount(); i++) {
        colors[i] = NONE;
    }
}

ColoredGraph::~ColoredGraph() {
    delete colors;
}

bool ColoredGraph::hasColor(int vertex) const {
    return colors[vertex - 1] != NONE;
}

Color ColoredGraph::getColor(int vertex) const {
    return colors[vertex - 1];
}

void ColoredGraph::setColor(int vertex, Color color) {
    colors[vertex - 1] = color;
}

bool ColoredGraph::allColored() const {
    for (int i = 0; i < getVertexCount(); i++) {
        if (colors[i] == NONE) {
            return false;
        }
    }
    return true;
}
