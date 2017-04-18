#include <mpi/mpi.h>

#include "Packer.h"
#include "Logger.h"

Packer::Packer(int graphSize) {
    this->graphSize = graphSize;
}

char * Packer::packGraph(Graph * graph) {
    //char * buffer = (char *)malloc(graphSize * sizeof(char)); // FIXME + array size
    char * buffer = new char[BUFFER_SIZE];
    int position = 0;

    if (PACKER_DEBUG) Logger::logLn("packing:");

    // flat array size
    int matrixArraySize = graph->_getMatrix()->_getArraySize();
    MPI_Pack(&matrixArraySize, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    // flat array
    bool * matrixArray = graph->_getMatrix()->_getArray();
    if (PACKER_DEBUG) {
        std::stringstream str;
        str << "adjacency matrix: ";
        for (int i = 0; i < matrixArraySize; i++) {
            str << matrixArray[i] << " ";
        }
        str << std::endl;
        Logger::log(&str);
    }
    MPI_Pack(matrixArray, matrixArraySize, MPI_CXX_BOOL, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    return buffer;
}

Graph * Packer::unpackGraph(char * packed) {
    int position = 0;
    if (PACKER_DEBUG) Logger::logLn("unpacking:");

    // flat array size
    int matrixArraySize = 0;
    MPI_Unpack(packed, BUFFER_SIZE, &position, &matrixArraySize, 1, MPI_INT, MPI_COMM_WORLD);
    if (PACKER_DEBUG) {
        std::stringstream str;
        str << "matrixArraySize is: " << matrixArraySize << std::endl;
        Logger::log(&str);
    }

    // flat array
    bool * matrixArray;
    MPI_Unpack(packed, BUFFER_SIZE, &position, matrixArray, 1, MPI_CXX_BOOL, MPI_COMM_WORLD);
    if (PACKER_DEBUG) {
        std::stringstream str;
        str << "adjacency matrix: ";
        for (int i = 0; i < matrixArraySize; i++) {
            str << matrixArray[i] << " ";
        }
        str << std::endl;
        Logger::log(&str);
    }

}
