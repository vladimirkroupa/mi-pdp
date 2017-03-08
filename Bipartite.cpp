#include <stdexcept>
#include "Bipartite.h"

Bipartite::Bipartite(const Graph &graph) {
    this->graph = new ColoredGraph(graph);
}

Bipartite::~Bipartite() {
    delete graph;
}

void Bipartite::dfs(int node, Color color) {
    if (!graph->hasColor(node)) {
        graph->setColor(node, color);
        const std::vector<int> * adjacent = graph->getAdjacentNodes(node);
        for (int i = 0; i < adjacent->size(); i++) {
            int adjacentNode = (*adjacent)[i];
            if (graph->hasColor(adjacentNode) && graph->getColor(adjacentNode) == color) {
                isBipartite = false;
                return;
            }
            dfs(adjacentNode, otherColor(color));
        }
    }
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
