#ifndef MI_PDP_CPP_PACKER_H
#define MI_PDP_CPP_PACKER_H

#include "Graph.h"

#define PACKER_DEBUG true
#define BUFFER_SIZE 20000

class Packer {
public:
    Packer(int graphSize);

    char * packGraph(Graph * graph, int rank);
    Graph * unpackGraph(char * packed, int rank);
private:
    int graphSize;
};

#endif //MI_PDP_CPP_PACKER_H
