#include "Worker.h"

Worker::Worker(Graph &problem, int threadsPerSolver) {

    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &commSize);
    MPI_Comm_rank(comm, &rank);
}

Worker::~Worker() {

}

void Worker::run() {

}

void Worker::runMaster() {
    MPI_Status status;
    for (int dest = 1; dest < commSize; dest++) { // pro vsechny slave procesy
        MPI_Send(..., dest, tag_work, MPI_COMM_WORLD); //pocatecnı distribuce prace
    }
    int working_slaves = num_procs - 1; // pocet pracujıcıch slave procesu
    while (working_slaves > 0) { // hlavnı smycka
        MPI_Recv(..., MPI_ANY_SOURCE, tag_done, MPI_COMM_WORLD, &status);
        if (more_work_to_be_done) // dokud existuje dalsı prace
            MPI_Send(..., status.MPI_SOURCE, tag_work, MPI_COMM_WORLD);
        else {
            MPI_Send(..., status.MPI_SOURCE, tag_finished, MPI_COMM_WORLD);
            working_slaves--;
        }
    }
}

void Worker::runSlave() {
    while (true) {
        MPI_Recv(..., 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == tag_finished) {
            break; // konec vypoctu
        }
        else if (status.MPI_TAG == tag_work) {
            // do the work

            MPI_Send(..., 0, tag_done, MPI_COMM_WORLD);
        }
    }
}