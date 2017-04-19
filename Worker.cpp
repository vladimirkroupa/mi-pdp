#include "Worker.h"
#include "Logger.h"
#include "Solver.h"

#include <sstream>

Worker::Worker(Graph &problem, int threadsPerSolver) {
    packer = new Packer(problem.getSize());

    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &commSize);
    MPI_Comm_rank(comm, &rank);

    _problem = &problem;
}

Worker::~Worker() {
    delete packer;
    delete _problem;
}

void Worker::run() {
    if (rank == 0) {
        runMaster();
    } else {
        runSlave();
    }
}

void Worker::runMaster() {
    MPI_Status status;
    int message = 0;
    for (int dest = 1; dest < commSize; dest++) { // pro vsechny slave procesy
        sendWork(*_problem, dest); // pocatecni distribuce prace
    }

    int workingSlaves = commSize - 1;
    while (workingSlaves > 0) { // hlavni smycka
        MPI_Recv(&message, BUFFER_SIZE, MPI_INT, MPI_ANY_SOURCE, DONE, MPI_COMM_WORLD, &status);
        int from = status.MPI_SOURCE;
        std::cout << "solution from "  << from << ": " << message << std::endl;
        if (false) { // FIXME if more work to be done
            sendWork(*_problem, from); // FIXME get work
        } else {
            MPI_Send(&message, 0, MPI_INT, from, TERMINATE, MPI_COMM_WORLD);
            workingSlaves--;
        }
    }
}

void Worker::runSlave() {
    while (true) {
        int tag;
        Graph *g = receiveWork(0, &tag);
        if (tag == TERMINATE) {
            std::cout << rank << " terminating..." << std::endl;
            break; // konec vypoctu
        } else if (tag == WORK_SHARE) {
            std::cout << rank << " working..." << std::endl;
            int solutionValue = solve(*g);
            MPI_Send(&solutionValue, 0, MPI_INT, 0, DONE, comm);
        }
    }
}

int Worker::solve(Graph &problem) {

    std::cout << problem << std::endl;

    Solver solver(problem, 2);
    solver.solve();
    Graph *solution = solver.getSolution();

    std::cout << "got solution";

    int result;
    if (solution != NULL) {
        result = solution->getEdgeCount();
        delete solution;
    }
    return result;
}

void Worker::sendWork(Graph & problem, int to) {
    int size = 0;
    char * message = packer->packGraph(&problem, &size, rank);
    MPI_Send(message, size, MPI_PACKED, to, WORK_SHARE, comm);
    if (MPI_DEBUG) { std::stringstream str; str << rank << " sent work to " << to << ", size: " << size << std::endl; Logger::log(&str, rank); }

    delete[] message;
}

Graph * Worker::receiveWork(int source, int * tag) {
    MPI_Status status;
    char * workBuffer = new char[BUFFER_SIZE];
    MPI_Recv(workBuffer, BUFFER_SIZE, MPI_PACKED, source, MPI_ANY_TAG, comm, &status);
    *tag = status.MPI_TAG;
    Graph * graph;
    if (*tag == WORK_SHARE) {
        if (MPI_DEBUG) { std::stringstream str; str << rank << " received work from " << source << std::endl; Logger::log(&str, rank); }
        graph = packer->unpackGraph(workBuffer, rank);
    }
    return graph;
}