#ifndef MI_PDP_CPP_MASTER_H
#define MI_PDP_CPP_MASTER_H

#include "Graph.h"
#include "Packer.h"
#include <mpi/mpi.h>
#include <deque>

#define WORK_SHARE 1
#define DONE 2
#define TERMINATE 3

#define MPI_DEBUG true

class Worker {
public:
    Worker(Graph &problem, int threadsPerSolver);

    virtual ~Worker();

    int solve(Graph &problem);
    void run();

private:
    void runMaster();
    void runSlave();

    Graph * nextUnitOfWork();
    void sendWork(Graph &problem, int to);
    Graph * receiveWork(int source, int * tag);

    std::deque<Graph *> _masterWork;
    Graph * _problem;

    int bestSolution;

    int threadsPerSolver;
    int rank;
    int commSize;
    MPI_Comm comm;
    Packer * packer;
};


#endif //MI_PDP_CPP_MASTER_H
