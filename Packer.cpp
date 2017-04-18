#include <mpi/mpi.h>

#include "Packer.h"
#include "Logger.h"

Packer::Packer(int graphSize) {
    this->graphSize = graphSize;
}

char * Packer::packWorkShare(Graph * graph) {
    char * buffer = (char *)malloc(graphSize * sizeof(char));
    int position = 0;

    if (PACKER_DEBUG) Logger::logLn("packing:");

    // adjacency matrix

    bool ** matrix = graph->getMatrix();
    if (PACKER_DEBUG) {
        stringstream str;
        str << "degrees: ";
        for (int i = 0; i < vertices; i++) {
            str << degrees[i] << " ";
        }
        str << endl;
        Logger::log(&str);
    }
    MPI_Pack(degrees, vertices, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    // marker
    char marker = '*';
    if (PACKER_DEBUG) Logger::logLn("*");
    MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    // edges
    vector<Edge> * edges = tree->getEdges();
    for(int i = 0; i < tree->edgeCount(); i++) {
        Edge edge = (*edges)[i];
        int edgeFrom = edge.vertex1;
        int edgeTo = edge.vertex2;
        MPI_Pack(&edgeFrom, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&edgeTo, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    }

    // marker
    MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    // stack_edges
    for(int i = 0; i < stackSize; i++) {
        Edge edge = (*stack)[i];
        int edgeFrom = edge.vertex1;
        int edgeTo = edge.vertex2;
        MPI_Pack(&edgeFrom, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&edgeTo, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    }

    // marker
    MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    *size = position;

    return buffer;
}

Graph *Packer::unpackWorkShare(char *packed) {
    return nullptr;
}
