#include <stdexcept>
#include "Bipartite.h"

Bipartite::Bipartite(Graph & _graph) : graph(_graph) {
    colors = new Color[graph.getSize()];
    for (int i = 0; i < graph.getSize(); i++) {
        colors[i] = NONE;
    }
    isBipartite = true;
    for (int i = 1; i <= graph.getSize(); i++) {
        dfs(i, GREEN);
    }
}

Bipartite::~Bipartite() {
    delete[] colors;
}

void Bipartite::dfs(int node, Color color) {
    if (!hasColor(node)) {
        setColor(node, color);
        const std::vector<int> adjacent = graph.getAdjacentNodes(node);
        for (unsigned int i = 0; i < adjacent.size(); i++) {
            int adjacentNode = adjacent[i];
            if (hasColor(adjacentNode) && getColor(adjacentNode) == color) {
                isBipartite = false;
                return;
            }
            dfs(adjacentNode, otherColor(color));
        }
    }
}

bool Bipartite::hasColor(int vertex) const {
    return colors[vertex - 1] != NONE;
}

Color Bipartite::getColor(int vertex) const {
    return colors[vertex - 1];
}

void Bipartite::setColor(int vertex, Color color) {
    colors[vertex - 1] = color;
}


Color Bipartite::otherColor(Color color) {
    if (color == NONE) {
        throw new std::invalid_argument("Cannot switch no color!");
    } else if (color == GREEN) {
        return BLUE;
    } else {
        return GREEN;
    }
}
