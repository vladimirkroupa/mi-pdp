#include <iostream>
#include <omp.h>
#include "Solver.h"
#include "Bipartite.h"

Solver::Solver(Graph &problem, int threads) {
    this->threads = threads;
    incumbent = nullptr;
    incumbentObjective = problem.getSize() - 1;
    _deque.push_back(&problem);
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
    doSolve(&_deque);
}

void Solver::doSolve(std::deque<Graph *> *deque) {
    # pragma omp parallel shared(deque) num_threads(threads)
    {
        printf("thread %i ready...\n", omp_get_thread_num());
        while (!deque->empty()) {
            Graph *g;
            # pragma omp critical
            {
                g = deque->back();
                deque->pop_back();
            }

            # pragma omp task
            solveState(deque, g);
            # pragma omp taskwait
        }
    }
}

void Solver::solveState(std::deque<Graph *> *deque, Graph *g) {

    if (printSkip == 100000) {
        printf("%i / deque size: %li / edge count: %i / max: %i\n", omp_get_thread_num(), deque->size(), g->getEdgeCount(), incumbentObjective);
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        printf("!! found solution with edge count %i \n", g->getEdgeCount());
        # pragma omp critical
        setIncumbent(g);
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            nextG->removeEdge(i);
            if (nextG->getEdgeCount() > incumbentObjective ||
                (incumbent == nullptr && g->getEdgeCount() == incumbentObjective)) {
                # pragma omp critical
                deque->push_back(nextG);
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

