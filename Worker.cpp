#include "Worker.h"
#include "Logger.h"

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
    int i = 50;
    for (int dest = 1; dest < commSize; dest++) { // pro vsechny slave procesy
        sendWork(*_problem, dest);
//        MPI_Send(&i, 1, MPI_INT, dest, WORK_SHARE, MPI_COMM_WORLD); //pocatecnı distribuce prace
    }
    // delete
    for (int dest = 1; dest < commSize; dest++) {
        MPI_Send(&i, 0, MPI_INT, dest, TERMINATE, MPI_COMM_WORLD);
    }

//    int working_slaves = num_procs - 1; // pocet pracujıcıch slave procesu
//    while (working_slaves > 0) { // hlavnı smycka
//        MPI_Recv(..., MPI_ANY_SOURCE, tag_done, MPI_COMM_WORLD, &status);
//        if (more_work_to_be_done) // dokud existuje dalsı prace
//            MPI_Send(..., status.MPI_SOURCE, tag_work, MPI_COMM_WORLD);
//        else {
//            MPI_Send(..., status.MPI_SOURCE, tag_finished, MPI_COMM_WORLD);
//            working_slaves--;
//        }
//    }
}

void Worker::runSlave() {
    while (true) {
        MPI_Status status;
        Graph * g = receiveWork(0);
        std::cout << "got graph of size:" << g->getSize() << std::endl;
        int message = -1;
//        MPI_Recv(&message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//        std::cout << rank << " got message: " << message << std::endl;

        MPI_Recv(&message, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TERMINATE) {
            std::cout << rank << " terminating..." << std::endl;
            break; // konec vypoctu
        }
//        else if (status.MPI_TAG == tag_work) {
//            // do the work
//
//            MPI_Send(..., 0, tag_done, MPI_COMM_WORLD);
//        }
    }
}

void Worker::sendWork(Graph & problem, int to) {
    int position = 0;
    char * message = packer->packGraph(&problem, rank);
    MPI_Send(message, position, MPI_PACKED, to, WORK_SHARE, comm);
    if (MPI_DEBUG) {
        std::stringstream str;
        str << rank << " sent work to " << to << std::endl;
        Logger::log(&str, rank);
    }

    delete[] message;
}

Graph * Worker::receiveWork(int source) {
    char * workBuffer = new char[BUFFER_SIZE];
    MPI_Recv(workBuffer, BUFFER_SIZE, MPI_PACKED, source, WORK_SHARE, comm, MPI_STATUS_IGNORE);
    if (MPI_DEBUG) { std::stringstream str; str << rank << " received work from " << source << std::endl; Logger::log(&str, rank); }
    Graph * graph = packer->unpackGraph(workBuffer, rank);

    if (MPI_DEBUG) {
        std::stringstream str; str << "received graph of size: " << graph->getSize() << std::endl; Logger::log(&str, rank);
    }

    return graph;
}