#include <mpi/mpi.h>

#include "Packer.h"
#include "Logger.h"

Packer::Packer(int graphSize) {
    this->graphSize = graphSize;
}

char * Packer::packGraph(Graph * graph, int * size, int rank) {
    //char * buffer = (char *)malloc(graphSize * sizeof(char)); // FIXME + array size
    char * buffer = new char[BUFFER_SIZE];
    *size = 0;

    if (PACKER_DEBUG) Logger::logLn("packing...", rank);

    // emulated size
    int emulatedSize = graph->_getMatrix()->size();
    if (PACKER_DEBUG) { std::stringstream str; str << "emulatedSize is: " << emulatedSize << std::endl; Logger::log(&str, rank); }
    MPI_Pack(&emulatedSize, 1, MPI_INT, buffer, BUFFER_SIZE, size, MPI_COMM_WORLD);

    // flat array size
    int matrixArraySize = graph->_getMatrix()->_getArraySize();
    if (PACKER_DEBUG) { std::stringstream str; str << "matrixArraySize is: " << matrixArraySize << std::endl; Logger::log(&str, rank); }
    MPI_Pack(&matrixArraySize, 1, MPI_INT, buffer, BUFFER_SIZE, size, MPI_COMM_WORLD);


    // flat array
    bool * matrixArray = graph->_getMatrix()->_getArray();
    if (PACKER_DEBUG) { std::stringstream str; str << "adjacency matrix: "; for (int i = 0; i < matrixArraySize; i++) { str << matrixArray[i] << " "; } str << std::endl; Logger::log(&str, rank); }
    MPI_Pack(matrixArray, matrixArraySize, MPI_C_BOOL, buffer, BUFFER_SIZE, size, MPI_COMM_WORLD);

    int idSize = graph->id.size();
    MPI_Pack(&idSize, 1, MPI_INT, buffer, BUFFER_SIZE, size, MPI_COMM_WORLD);

    int * idArr = &(graph->id)[0];
    MPI_Pack(idArr, idSize, MPI_INT, buffer, BUFFER_SIZE, size, MPI_COMM_WORLD);

    return buffer;
}

Graph * Packer::unpackGraph(char * packed, int rank) {
    int position = 0;
    if (PACKER_DEBUG) Logger::logLn("unpacking...", rank);

    // emulated size
    int emulatedSize = 0;
    MPI_Unpack(packed, BUFFER_SIZE, &position, &emulatedSize, 1, MPI_INT, MPI_COMM_WORLD);
    if (PACKER_DEBUG) { std::stringstream str; str << "emulatedSize is: " << emulatedSize << std::endl; Logger::log(&str, rank); }

    // flat array size
    int matrixArraySize = 0;
    MPI_Unpack(packed, BUFFER_SIZE, &position, &matrixArraySize, 1, MPI_INT, MPI_COMM_WORLD);
    if (PACKER_DEBUG) { std::stringstream str; str << "matrixArraySize is: " << matrixArraySize << std::endl; Logger::log(&str, rank); }

//    // flat array
    bool * matrixArray = new bool[matrixArraySize];
    MPI_Unpack(packed, BUFFER_SIZE, &position, matrixArray, matrixArraySize, MPI_CXX_BOOL, MPI_COMM_WORLD);
    if (PACKER_DEBUG) {
        std::stringstream str;
        str << "adjacency matrix: ";
        for (int i = 0; i < matrixArraySize; i++) {
            str << matrixArray[i] << " ";
        }
        str << std::endl;
        Logger::log(&str, rank);
    }

    int idSize = 0;
    MPI_Unpack(packed, BUFFER_SIZE, &position, &idSize, 1, MPI_INT, MPI_COMM_WORLD);

    int * id = new int[idSize];
    MPI_Unpack(packed, BUFFER_SIZE, &position, id, idSize, MPI_INT, MPI_COMM_WORLD);

    AdjacencyMatrix * adjacencyMatrix = new AdjacencyMatrix(matrixArray, matrixArraySize, emulatedSize);
    Graph * g = new Graph(adjacencyMatrix);

    for (int i = 0; i < idSize; i++) {
        g->id.push_back(id[i]);
    }

    return g;
}
