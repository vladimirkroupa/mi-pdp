#include <iostream>
#include <omp.h>
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

void Solver::setIncumbent(Graph *graph) {
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

void Solver::doSolve(std::stack<Graph *> *stack) {
    # pragma omp parallel shared(stack) num_threads(4)
    {
        printf("thread %i ready...\n", omp_get_thread_num());
        while (!stack->empty()) {
            Graph *g;
            # pragma omp critical
            {
                g = stack->top();
                stack->pop();
            }

            # pragma omp task
            solveState(stack, g);
            # pragma omp taskwait
        }
    }
}

void Solver::solveState(std::stack<Graph *> *stack, Graph *g) {

    if (printSkip == 100000) {
        printf("%i / stack size: %li / edge count: %i / max: %i\n", omp_get_thread_num(), stack->size(), g->getEdgeCount(), incumbentObjective);
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        printf("!! found solution with edge count %i \n", g->getEdgeCount());
        setIncumbent(g);
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            nextG->removeEdge(i);
            if (nextG->getEdgeCount() > incumbentObjective ||
                (incumbent == nullptr && g->getEdgeCount() == incumbentObjective)) {
                # pragma omp critical
                stack->push(nextG);
            } else {
                delete nextG;
            }
        }
        delete g;
    }
}

bool Solver::isBipartite(Graph &graph) const {
    Bipartite *bp = new Bipartite(graph);
    bool result = bp->isBipartite;
    delete bp;
    return result;
}

