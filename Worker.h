#ifndef MI_PDP_CPP_MASTER_H
#define MI_PDP_CPP_MASTER_H

#include "Graph.h"
#include <mpi/mpi.h>

class Worker {
public:
    Worker(Graph &problem, int threadsPerSolver);

    virtual ~Worker();

    void run();

private:
    void runMaster();
    void runSlave();

    int rank;
    int commSize;
    MPI_Comm comm;
};


#endif //MI_PDP_CPP_MASTER_H
