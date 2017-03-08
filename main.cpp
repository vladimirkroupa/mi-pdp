#include <clocale>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "Graph.h"
#include "Solver.h"

Graph *createGraph(char *filename) {
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


int main() {
    Graph *graph = createGraph("../graph10_5.txt");
    Solver *solver = new Solver(*graph);
    solver->solve();
    Graph *solution = solver->getSolution();
    if (solution != NULL) {
        std::cout << "Result: " << solution->getEdgeCount() << std::endl;
    } else {
        std::cout << "No solution." << std::endl;
    }

}