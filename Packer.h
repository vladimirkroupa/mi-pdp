#ifndef MI_PDP_CPP_PACKER_H
#define MI_PDP_CPP_PACKER_H

#include "Graph.h"

#define PACKER_DEBUG false

class Packer {
public:
    Packer(int graphSize);

    char * packWorkShare(Graph *graph);
    Graph * unpackWorkShare(char * packed);
private:
    int graphSize;
};

#endif //MI_PDP_CPP_PACKER_H
