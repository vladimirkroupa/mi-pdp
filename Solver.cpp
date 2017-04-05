#include <iostream>
#include <omp.h>
#include "Solver.h"
#include "Bipartite.h"

Solver::Solver(Graph &problem, int threads) {
    this->threads = threads;
    incumbent = nullptr;
    incumbentObjective = problem.getSize() - 1;
    for (int i = 1; i <= problem.getEdgeCount(); i++) {
        Graph *nextG = new Graph(problem);
        nextG->removeEdge(i);
        _deque1.push_back(nextG);
    }
    delete problem;
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
    # pragma omp parallel num_threads(threads)
    printf("thread %i ready...\n", omp_get_thread_num());
    while (!_deque1.empty()) {
        std::cout << "switching queues..." << _deque1.size() << std::endl;
        doSolve(&_deque1, &_deque2);
        _deque1.clear();
        std::cout << "switching queues..." << _deque2.size() << std::endl;
        doSolve(&_deque2, &_deque1);
        _deque2.clear();
    }
}

void Solver::doSolve(std::deque<Graph *> *workDeque, std::deque<Graph *> *auxDeque) {
    #pragma omp parallel for
    for (unsigned int i = 0; i < workDeque->size(); i++) {
        Graph *g = (*workDeque)[i];
        if (possiblyBetter(g)) {
            solveState(auxDeque, g);
        } else {
            delete g;
        }
    }
}

void Solver::solveState(std::deque<Graph *> *auxDeque, Graph *g) {

    if (printSkip == 100000) {
        printf("%i / edge count: %i / max: %i\n", omp_get_thread_num(), g->getEdgeCount(), incumbentObjective);
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        if (g->getEdgeCount() > incumbentObjective) {
            printf("!! found solution with edge count %i \n", g->getEdgeCount());
            # pragma omp critical
            setIncumbent(g);
        } else {
            delete g;
        }
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            nextG->removeEdge(i);
            if (possiblyBetter(nextG)) {
                # pragma omp critical
                auxDeque->push_back(nextG);
            } else {
                delete nextG;
            }
        }
        delete g;
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

