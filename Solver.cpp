#include <iostream>
#include "Solver.h"
#include "Bipartite.h"

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif

Solver::Solver(Graph *problem, int threads) {
    this->threads = threads;
    incumbent = nullptr;
    incumbentObjective = problem->getSize() - 1;
    _deque.push_back(problem);
}

Solver::~Solver() {
    if (incumbent != nullptr) {
        delete incumbent;
        counters.graphDestroyed();
    }
}

void Solver::setIncumbent(Graph *graph) {
    if (incumbent != nullptr) {
        delete incumbent;
        counters.graphDestroyed();
    }
    incumbent = graph;
    incumbentObjective = graph->getEdgeCount();
}

Graph *Solver::getSolution() const {
    return incumbent;
}

void Solver::solve() {
    # pragma omp parallel num_threads(threads)
    printf("thread %i ready...\n", omp_get_thread_num());
    while (!_deque.empty()) {
        doSolve(&_deque, 10000);
        std::cout << "remaining: " << _deque.size() << std::endl;
    }
}

void Solver::doSolve(std::vector<Graph *> *deque, int statesToSolve) {
    unsigned int workSteps = statesToSolve < deque->size() ? statesToSolve : deque->size();
    #pragma omp parallel for
    for (unsigned int i = 0; i < workSteps; i++) {
        Graph *g = (*deque)[i];
        //std::cout << "solving graph " << g->getId() << " " << std::endl;
        if (possiblyBetter(g)) {
            solveState(deque, g);
        } else {
            delete g;
            counters.graphDestroyed();
        }
    }
    deque->erase(deque->begin() + 0, deque->begin() + workSteps);
}

void Solver::solveState(std::vector<Graph *> *deque, Graph *g) {

    if (printSkip == 100000) {
        std::cout << omp_get_thread_num() << " / edge count: " << g->getEdgeCount() << " / max: " << incumbentObjective << std::endl;
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        if (g->getEdgeCount() > incumbentObjective) {
            std::cout << "!! found solution with edge count " << g->getEdgeCount() << std::endl;
            # pragma omp critical
            setIncumbent(g);
        } else {
            delete g;
            counters.graphDestroyed();
        }
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            counters.graphCreated();
            nextG->removeEdge(i);
            if (possiblyBetter(nextG)) {
                # pragma omp critical
                deque->push_back(nextG);
            } else {
                delete nextG;
                counters.graphDestroyed();
                
            }
        }
        delete g;
        counters.graphDestroyed();
    }
}

bool Solver::possiblyBetter(Graph * graph) const {
    return graph->getEdgeCount() > incumbentObjective ||
            (incumbent == nullptr && graph->getEdgeCount() == incumbentObjective);
}

bool Solver::isBipartite(Graph & graph) const {
    Bipartite *bp = new Bipartite(graph);
    bool result = bp->isBipartite;
    delete bp;
    return result;
}
