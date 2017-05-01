#ifndef MI_PDP_CPP_ADJACENCYMATRIX_H
#define MI_PDP_CPP_ADJACENCYMATRIX_H


class AdjacencyMatrix {
public:
    AdjacencyMatrix(int size);
    AdjacencyMatrix(const AdjacencyMatrix& orig);
    virtual ~AdjacencyMatrix();
    bool get(int node1, int node2) const;
    bool set(int node1, int node2, bool value);
    int size();
private:
    bool * flatArray;
    int arraySize;
    int emulatedSize;
    int flatIndex(int row, int col) const;
};


#endif //MI_PDP_CPP_ADJACENCYMATRIX_H
