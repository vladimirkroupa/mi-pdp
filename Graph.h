#ifndef MI_PDP_CPP_GRAPH_H
#define MI_PDP_CPP_GRAPH_H

#include <vector>

class Graph {
public:
    Graph(int size);
    Graph(const Graph& graph);
    virtual ~Graph();
    int getVertexCount() const;
    int getEdgeCount() const;
    bool hasEdge(int vertex1, int vertex2) const;
    void addEdge(int vertex1, int vertex2);
    void removeEdge(int vertex1, int vertex2);
    void removeEdge(int edgeNo);
    void removeNextEdge();
    bool isBipartite() const;

    std::vector<int> * getAdjacentNodes(int vertex) const;
    //friend std::ostream & operator <<(std::ostream & os, const Graph & graph);
protected:
    void checkVertexExists(int node) const;
private:
    bool ** adjacencyMatrix;
    int matrixSize;
    int edgeCount;

    void recomputeEdgeCount();
};

#endif
