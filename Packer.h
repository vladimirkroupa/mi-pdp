#ifndef MI_PDP_CPP_PACKER_H
#define MI_PDP_CPP_PACKER_H

#include "Graph.h"

#define PACKER_DEBUG false
#define BUFFER_SIZE 2000

class Packer {
public:
    Packer(int graphSize);

    char * packGraph(Graph *graph);
    Graph * unpackGraph(char * packed);
private:
    int graphSize;
};

#endif //MI_PDP_CPP_PACKER_H
