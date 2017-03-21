#include <clocale>
#include <stdexcept>
#include <assert.h>
#include <iostream>
#include "Graph.h"
#include "Bipartite.h"

Graph::Graph(int size) {
    matrixSize = size - 1;
    edgeCount = 0;
    adjacencyMatrix = new bool* [matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        int elemsInRow = matrixSize - i;
        adjacencyMatrix[i] = new bool[elemsInRow];
        std::fill_n(adjacencyMatrix[i], elemsInRow, false);
    }
}

Graph::Graph(const Graph& graph) : Graph(graph.getSize()) {
    for (int i = 1; i < graph.getSize(); i++) {
        for (int j = i + 1; j <= graph.getSize(); j++) {
            bool connected = graph.hasEdge(i, j);
            if (connected) {
                addEdge(i, j);
            }
        }
    }
}

Graph::~Graph() {
    for (int i = 0; i < matrixSize; i++) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;
}

std::vector<int> Graph::getAdjacentNodes(int node) const {
    std::vector<int> edges;
    for (int node2 = 1; node2 <= matrixSize; node2++) {
        if (hasEdge(node, node2)) {
            edges.push_back(node2);
        }
    }
    return edges;
}

int Graph::getSize() const {
    return matrixSize + 1;
}

int Graph::getEdgeCount() const {
    return edgeCount;
}

bool Graph::hasEdge(int node1, int node2) const {
//    checkNodeExists(node1);
//    checkNodeExists(node2);
    return get(node1, node2);
}

void Graph::addEdge(int node1, int node2) {
//    checkNodeExists(node1);
//    checkNodeExists(node2);
    bool before = set(node1, node2, true);
    if (!before) {
        edgeCount += 1;
    }
}

void Graph::removeEdge(int node1, int node2) {
//    checkNodeExists(node1);
//    checkNodeExists(node2);
    bool removed = set(node1, node2, false);
    if (removed) {
        edgeCount -= 1;
    }
}

void Graph::removeEdge(int edgeNo) {
    int skipped = 0;
    for (int i = 1; i <= getSize(); i++) {
        for (int j = i; j <= getSize(); j++) {
            if (hasEdge(i, j)) {
                if (skipped == edgeNo - 1) {
                    removeEdge(i, j);
                    return;
                } else {
                    skipped++;
                }
            }
        }
    }
}

void Graph::removeNextEdge() {
    removeEdge(1);
}

bool Graph::get(int node1, int node2) const {
    if (node1 == node2) {
        return false;
    }
    if (node1 > node2) {
        return get(node2, node1);
    }
    int colIx = node2 - node1 - 1;
    int rowIx = node1 - 1;
    return adjacencyMatrix[rowIx][colIx];
}

bool Graph::set(int node1, int node2, bool value) {
    if (node1 == node2) {
        assert(!value);
        return false;
    }
    if (node1 > node2) {
        return set(node2, node1, value);
    }
    int colIx = node2 - node1 - 1;
    int rowIx = node1 - 1;
    bool before = adjacencyMatrix[rowIx][colIx];
    adjacencyMatrix[rowIx][colIx] = value;
    return before;
}

void Graph::checkNodeExists(int node) const {
    int nodeIx = node - 1;
    if (nodeIx < 0 || nodeIx > matrixSize - 1) {
        throw new std::out_of_range("No such node " + node);
    }
}
