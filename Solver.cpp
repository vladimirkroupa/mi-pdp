#include <iostream>
#include "Solver.h"
#include "Bipartite.h"

Solver::Solver(Graph &problem) {
    incumbent = nullptr;
    incumbentObjective = problem.getSize() - 1;
    _stack.push(&problem);
}

Solver::~Solver() {
    if (incumbent != nullptr) {
        delete incumbent;
    }
}

void Solver::setIncumbent(Graph * graph) {
    if (incumbent != nullptr) {
        delete incumbent;
    }
    incumbent = graph;
    incumbentObjective = graph->getEdgeCount();
}

Graph *Solver::getSolution() const {
    return incumbent;
}

void Solver::solve() {
    doSolve(&_stack);
}

void Solver::doSolve(std::stack<Graph *> * stack) {
    int printSkip = 0;
    while (!stack->empty()) {
        Graph *g = stack->top();
        stack->pop();

        if (printSkip == 100000) {
            std::cout << "stack size: " << stack->size() << " / edge count: " << g->getEdgeCount() << " / max: "
                      << incumbentObjective << std::endl;
            printSkip = 0;
        } else {
            printSkip++;
        }

        solveState(stack, g);
    }
}


void Solver::solveState(std::stack<Graph *> * stack, Graph * g) {
    if (isBipartite(*g)) {
        std::cout << "!! found solution with edge count " << g->getEdgeCount() << std::endl;
        setIncumbent(g);
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            nextG->removeEdge(i);
            if (nextG->getEdgeCount() > incumbentObjective ||
                (incumbent == nullptr && g->getEdgeCount() == incumbentObjective)) {
                stack->push(nextG);
            } else {
                delete nextG;
            }
        }
        delete g;
    }
}

bool Solver::isBipartite(Graph & graph) const {
    Bipartite *bp = new Bipartite(graph);
    bool result = bp->isBipartite;
    delete bp;
    return result;
}

