#ifndef MI_PDP_CPP_GRAPH_H
#define MI_PDP_CPP_GRAPH_H

#include <vector>

class Graph {
public:
    Graph(int size);
    Graph(const Graph& graph);
    virtual ~Graph();
    int getSize() const;
    int getEdgeCount() const;
    bool hasEdge(int node1, int node2) const;
    void addEdge(int node1, int node2);
    void removeEdge(int node1, int node2);
    void removeEdge(int edgeNo);
    void removeNextEdge();

    std::vector<int> getAdjacentNodes(int node) const;
    //friend std::ostream & operator <<(std::ostream & os, const Graph & graph);
protected:
    void checkNodeExists(int node) const;
private:
    bool ** adjacencyMatrix;
    int matrixSize;
    int edgeCount;
    bool get(int node1, int node2) const;
    bool set(int node1, int node2, bool value);
};

#endif
