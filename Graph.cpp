#include <clocale>
#include <stdexcept>
#include "Graph.h"
#include "Bipartite.h"

Graph::Graph(int size) {
    this->matrixSize = size;
    this->adjacencyMatrix = new bool* [matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        adjacencyMatrix[i] = new bool[matrixSize];
        std::fill_n(adjacencyMatrix[i], matrixSize, 0);
    }
    recomputeEdgeCount();
}

Graph::Graph(const Graph& graph) {
    this->matrixSize = graph.matrixSize;
    this->adjacencyMatrix = new bool* [matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        adjacencyMatrix[i] = new bool[matrixSize];
        std::copy(graph.adjacencyMatrix[i], graph.adjacencyMatrix[i] + matrixSize, adjacencyMatrix[i]);
    }
    recomputeEdgeCount();
}

Graph::~Graph() {
    for (int i = 0; i < matrixSize; i++) {
        delete [] adjacencyMatrix[i];
    }
    delete [] adjacencyMatrix;
}

int Graph::getVertexCount() const {
    return matrixSize;
}

int Graph::getEdgeCount() const {
    return edgeCount;
}

bool Graph::hasEdge(int vertex1, int vertex2) const {
//    checkVertexExists(vertex1);
//    checkVertexExists(vertex2);
    return adjacencyMatrix[vertex1 - 1][vertex2 - 1];
}

void Graph::addEdge(int vertex1, int vertex2) {
//    checkVertexExists(vertex1);
//    checkVertexExists(vertex2);
    bool added = !adjacencyMatrix[vertex1 - 1][vertex2 - 1];
    adjacencyMatrix[vertex1 - 1][vertex2 - 1] = true;
    adjacencyMatrix[vertex2 - 1][vertex1 - 1] = true;
    if (added) {
        edgeCount++;
    }
}

void Graph::removeEdge(int vertex1, int vertex2) {
//    checkVertexExists(vertex1);
//    checkVertexExists(vertex2);
    bool removed = adjacencyMatrix[vertex1 - 1][vertex2 - 1];
    adjacencyMatrix[vertex1 - 1][vertex2 - 1] = false;
    adjacencyMatrix[vertex2 - 1][vertex1 - 1] = false;
    if (removed) {
        edgeCount--;
    }
}

void Graph::removeEdge(int edgeNo) {
    int skipped = 0;
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = i; j <= matrixSize; j++) {
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
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = i; j <= matrixSize; j++) {
            if (hasEdge(i, j)) {
                removeEdge(i, j);
                return;
            }
        }
    }
}

bool Graph::isBipartite() const {
    Bipartite * bp = new Bipartite(*this);
    bool result = bp->isBipartite;
    delete bp;
    return result;
}

std::vector<int> *Graph::getAdjacentNodes(int vertex) const {
    std::vector<int> * edges = new std::vector<int>();
    for (int vertex2 = 1; vertex2 <= matrixSize; vertex2++) {
        if (hasEdge(vertex, vertex2)) {
            edges->push_back(vertex2);
        }
    }
    return edges;
}

void Graph::checkVertexExists(int node) const {
    int nodeIx = node - 1;
    if (nodeIx < 0 || nodeIx > matrixSize - 1) {
        throw new std::out_of_range("No such node " + node);
    }
}

void Graph::recomputeEdgeCount() {
    int result = 0;
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = i; j <= matrixSize; j++) {
            if (hasEdge(i, j)) {
                result += 1;
            }
        }
    }
    edgeCount = result;
}
