#include <clocale>
#include <cstdlib>
#include <iostream>
#include "Graph.h"
#include "Solver.h"
#include "Worker.h"
#include "Logger.h"

Graph *createGraph(const char *filename) {
    FILE *file = fopen(filename, (const char *) "r");
    if (file == NULL) {
        throw std::invalid_argument("File does not exist.");
    }

    int vertexCount = 0;
    fscanf(file, "%d", &vertexCount);

    Graph *graph = new Graph(vertexCount);

    char token;
    int x = 0, y = 0, i = 0;
    while (fscanf(file, "%c", &token) == 1) {
        if (token == '\r') {
            continue;
        }
        if (token == '\n') {
            if (i > 0) {
                x = 0;
                y++;
            }

            continue;
        }
        if (token == '1') {
            graph->addEdge(x + 1, y + 1);
        }
        x++;
        i++;
    }
    fclose(file);
    return graph;
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    Graph *graph;
    int threads;
    try {
        std::string file = argv[1];
        threads = atoi(argv[2]);
        graph = createGraph(file.c_str());
    } catch (const std::invalid_argument & exception) {
        std::cout << "File does not exist." << std::endl;
        return -1;
    }

    Worker w = Worker(*graph, threads);
    w.run();

//    Solver solver(*graph, threads);
//    solver.solve();
//    Graph *solution = solver.getSolution();
//    if (solution != NULL) {
//        std::cout << "Result: " << solution->getEdgeCount() << std::endl;
//    } else {
//        std::cout << "No solution." << std::endl;
//    }
    MPI_Finalize();
}