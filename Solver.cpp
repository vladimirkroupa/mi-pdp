#include <iostream>
#include "Solver.h"
#include "Bipartite.h"
#include "Logger.h"

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif

#define SOLVER_DEBUG true

Solver::Solver(Graph *problem, int threads, int rank) {
    this->threads = threads;
    this->rank = rank;
    incumbent = nullptr;
    incumbentObjective = problem->getSize() - 1;
    _deque.push_back(problem);
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
    if (SOLVER_DEBUG) { std::stringstream str; str << "thread " << omp_get_thread_num() <<  "ready..." << std::endl; Logger::log(&str, rank); }
    while (!_deque.empty()) {
        doSolve(&_deque, 10000);
    }
}

void Solver::doSolve(std::vector<Graph *> *deque, int statesToSolve) {
    workSteps = statesToSolve < deque->size() ? statesToSolve : deque->size();
    #pragma omp parallel for
    for (unsigned int i = 0; i < workSteps; i++) {
        Graph *g = (*deque)[i];
        //std::cout << "solving graph " << g->getId() << " " << std::endl;
        if (possiblyBetter(g)) {
            solveState(deque, g);
        } else {
            delete g;
        }
    }
    deque->erase(deque->begin() + 0, deque->begin() + workSteps);
}

void Solver::solveState(std::vector<Graph *> *deque, Graph *g) {

    if (printSkip == 100000) {
        if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " stack size: " << deque->size() << " / edge count: " << g->getEdgeCount() << " / max: " << incumbentObjective << std::endl; Logger::log(&str, rank); }
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        if (g->getEdgeCount() > incumbentObjective) {
            if (SOLVER_DEBUG) { std::stringstream str; str << rank <<  "/" << omp_get_thread_num() << " found solution with edge count " << g->getEdgeCount() << std::endl; Logger::log(&str, rank); }
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
                deque->insert(deque->begin() + workSteps, nextG);
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
