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

const bool ColoredGraph::hasColor(int vertex) {
    return colors[vertex - 1] != NONE;
}

const Color ColoredGraph::getColor(int vertex) {
    return colors[vertex - 1];
}

void ColoredGraph::setColor(int vertex, Color color) {
    colors[vertex - 1] = color;
}

const bool ColoredGraph::allColored() {
    for (int i = 0; i < getVertexCount(); i++) {
        if (colors[i] == NONE) {
            return false;
        }
    }
    return true;
}
