#include <algorithm>
#include "AdjacencyMatrix.h"

AdjacencyMatrix::AdjacencyMatrix(int size) {
    this->emulatedSize = size;
    this->arraySize = size * (size - 1) / 2;
    this->flatArray = new bool[arraySize];
    std::fill_n(flatArray, arraySize, 0);
}

AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix &orig) {
    this->emulatedSize = orig.emulatedSize;
    this->arraySize = orig.arraySize;
    bool * arrayCopy = new bool[arraySize];
    std::copy(orig.flatArray, orig.flatArray + orig.arraySize, arrayCopy);
    this->flatArray = arrayCopy;
}

AdjacencyMatrix::~AdjacencyMatrix() {
    delete [] flatArray;
}

bool AdjacencyMatrix::get(int node1, int node2) const {
    if (node1 == node2) {
        return false;
    }
    if (node1 > node2) {
        return get(node2, node1);
    }
    int colIx = node2 - node1 - 1;
    int rowIx = node1 - 1;
    int flatIx = flatIndex(rowIx, colIx);
    return flatArray[flatIx];
}

bool AdjacencyMatrix::set(int node1, int node2, bool value) {
    if (node1 == node2) {
        return false;
    }
    if (node1 > node2) {
        return set(node2, node1, value);
    }
    int colIx = node2 - node1 - 1;
    int rowIx = node1 - 1;
    int flatIx = flatIndex(rowIx, colIx);
    bool before = flatArray[flatIx];
    flatArray[flatIx] = value;
    return before;
}

int AdjacencyMatrix::size() {
    return this->emulatedSize;
}

int AdjacencyMatrix::flatIndex(int row, int col) const {
    return this->emulatedSize * row + col - (row * (row + 1) / 2);
}

bool * AdjacencyMatrix::_getArray() {
    return flatArray;
}

int AdjacencyMatrix::_getArraySize() {
    return arraySize;
}
