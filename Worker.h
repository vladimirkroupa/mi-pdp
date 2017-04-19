#ifndef MI_PDP_CPP_MASTER_H
#define MI_PDP_CPP_MASTER_H

#include "Graph.h"
#include "Packer.h"
#include <mpi/mpi.h>

#define WORK_SHARE 1
#define TERMINATE 2

#define MPI_DEBUG true

class Worker {
public:
    Worker(Graph &problem, int threadsPerSolver);

    virtual ~Worker();

    void run();

private:
    void runMaster();
    void runSlave();

    void sendWork(Graph &problem, int to);
    Graph * receiveWork(int source);

    Graph * _problem;

    int rank;
    int commSize;
    MPI_Comm comm;
    Packer * packer;
};


#endif //MI_PDP_CPP_MASTER_H
