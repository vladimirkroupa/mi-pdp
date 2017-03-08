#include <iostream>
#include "Solver.h"

Solver::Solver(Graph &problem) {
    incumbent = NULL;
    incumbentObjective = problem.getVertexCount() - 1;
    stack = new std::stack<Graph*>();
    stack->push(&problem);
}

Solver::~Solver() {
    delete stack;
    delete incumbent;
}

void Solver::setIncumbent(Graph *graph) {
    this->incumbent = graph;
    this->incumbentObjective = graph->getEdgeCount();
}

Graph *Solver::getSolution() const {
    return incumbent;
}

void Solver::solve() {
    int printSkip = 0;
    while (!stack->empty()) {
        Graph *g = stack->top();
        stack->pop();

        if (printSkip == 1000) {
            std::cout << "stack size: " << stack->size() << " / edge count: " << g->getEdgeCount() << " / max: "
                      << incumbentObjective << std::endl;
            printSkip = 0;
        } else {
            printSkip++;
        }

        if (g->isBipartite()) {
            setIncumbent(g);
        } else {
            for (int i = 1; i <= g->getEdgeCount(); i++) {
                Graph *nextG = new Graph(*g);
                nextG->removeEdge(i);
                if (nextG->getEdgeCount() > incumbentObjective ||
                    (incumbent == NULL && g->getEdgeCount() == incumbentObjective)) {
                    stack->push(nextG);
                }
            }
        }

        delete g;
    }
}
