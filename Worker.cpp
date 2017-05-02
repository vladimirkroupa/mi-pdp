#include "Worker.h"
#include "Logger.h"
#include "Solver.h"

#include <sstream>

Worker::Worker(Graph &problem, int threadsPerSolver) {
    this->threadsPerSolver = threadsPerSolver;
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
    bestSolution = -1;
    if (rank == 0) {

        for (int i = 1; i <= _problem->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*_problem);
            nextG->removeEdge(i);
            _masterWork.push_back(nextG);
        }

        runMaster();
    } else {
        runSlave();
    }
}

Graph *Worker::nextUnitOfWork() {
    Graph * work = _masterWork.front();
    _masterWork.pop_front();
    return work;
}

void Worker::runMaster() {
    MPI_Status status;
    int message = -2;
    for (int dest = 1; dest < commSize; dest++) { // pro vsechny slave procesy
        Graph * initialWork = nextUnitOfWork();
        sendWork(*initialWork, dest); // pocatecni distribuce prace
        std::cout << "remaining work: " << _masterWork.size() << std::endl;
    }

    int workingSlaves = commSize - 1;
    while (workingSlaves > 0) { // hlavni smycka
        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, DONE, MPI_COMM_WORLD, &status);
        int from = status.MPI_SOURCE;
        if (MPI_DEBUG) { std::stringstream str; str << "solution from "  << from << ": " << message << std::endl; Logger::log(&str, rank); }
        if (message > bestSolution) {
            bestSolution = message;
        }
        if (! _masterWork.empty()) { // if more work to be done
            Graph * work = nextUnitOfWork();
            sendWork(*work, from); // get work
            std::cout << "remaining work: " << _masterWork.size() << std::endl;
        } else {
            if (MPI_DEBUG) { std::stringstream str; str << rank << " has no more work, asking slaves to terminate..." << std::endl; Logger::log(&str, rank); }
            MPI_Send(&message, 0, MPI_INT, from, TERMINATE, MPI_COMM_WORLD);
            workingSlaves--;
        }
    }
    std::cout << "Best solution: " << bestSolution << std::endl;
}

void Worker::runSlave() {
    while (true) {
        int tag;
        Graph *g = receiveWork(0, &tag);
        if (tag == TERMINATE) {
            if (MPI_DEBUG) { std::stringstream str; str << rank << " is terminating..."  << std::endl; Logger::log(&str, rank); }
            break; // konec vypoctu
        } else if (tag == WORK_SHARE) {
            if (MPI_DEBUG) { std::stringstream str; str << rank << " received work, id: " << g->getId() << std::endl; Logger::log(&str, rank); }
            int solutionValue = solve(*g);
            if (MPI_DEBUG) { std::stringstream str; str << rank << " found solution with value "  << solutionValue << std::endl; Logger::log(&str, rank); }
            MPI_Send(&solutionValue, 1, MPI_INT, 0, DONE, comm);
        }
    }
}

int Worker::solve(Graph &problem) {
//    std::cout << problem << std::endl;

    Solver solver(problem, threadsPerSolver, rank);
    solver.solve();
    Graph *solution = solver.getSolution();
    if (solution != NULL) {
        if (MPI_DEBUG) { std::stringstream str; str << rank << " found solution: " << solution->getEdgeCount() << std::endl; Logger::log(&str, rank); }
        return solution->getEdgeCount();
    } else {
        if (MPI_DEBUG) { std::stringstream str; str << rank << " hasn't found a solution..." << std::endl; Logger::log(&str, rank); }
        return -1;
    }
}

void Worker::sendWork(Graph & problem, int to) {
    int size = 0;
    char * message = packer->packGraph(&problem, &size, rank);
    MPI_Send(message, size, MPI_PACKED, to, WORK_SHARE, comm);
    if (MPI_DEBUG) { std::stringstream str; str << rank << " sent work to " << to << ", id: " << problem.getId() << std::endl; Logger::log(&str, rank); }

    delete[] message;
}

Graph * Worker::receiveWork(int source, int * tag) {
    MPI_Status status;
    char * workBuffer = new char[BUFFER_SIZE];
    MPI_Recv(workBuffer, BUFFER_SIZE, MPI_PACKED, source, MPI_ANY_TAG, comm, &status);
    *tag = status.MPI_TAG;
    Graph * graph;
    if (*tag == WORK_SHARE) {
        graph = packer->unpackGraph(workBuffer, rank);
    }
    return graph;
}