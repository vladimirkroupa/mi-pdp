#include <iostream>
#include "Solver.h"
#include "Bipartite.h"
#include "Logger.h"

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif

Solver::Solver(Graph &problem, int threads, int rank) {
    this->threads = threads;
    this->rank = rank;
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
    # pragma omp parallel shared(stack) num_threads(threads)
    {
//        printf("thread %i ready...\n", omp_get_thread_num());
        while (!stack->empty()) {
            Graph *g;
            # pragma omp critical
            {
                g = stack->top();
                stack->pop();
                if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " about to solve " << g->getId() << std::endl; Logger::log(&str, rank); }
            }

            if (possiblyBetter(g)) {
                # pragma omp task
                solveState(stack, g);
                # pragma omp taskwait
            } else {
                if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " is ignoring " << g->getId() << std::endl; Logger::log(&str, rank); }
                delete g;
            }
        }
        if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " stack is empty" << std::endl; Logger::log(&str, rank); }
    }
}

void Solver::solveState(std::stack<Graph *> *stack, Graph *g) {

    if (printSkip == 10000) {
        if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " stack size: " << stack->size() << " / edge count: " << g->getEdgeCount() << " / max: " << incumbentObjective << std::endl; Logger::log(&str, rank); }
        printSkip = 0;
    }
    printSkip++;

    if (isBipartite(*g)) {
        if (SOLVER_DEBUG) { std::stringstream str; str << rank <<  "/" << omp_get_thread_num() << " found solution with edge count " << g->getEdgeCount() << std::endl; Logger::log(&str, rank); }
        # pragma omp critical
        setIncumbent(g);
    } else {
        for (int i = 1; i <= g->getEdgeCount(); i++) {
            Graph *nextG = new Graph(*g);
            nextG->removeEdge(i);
            if (possiblyBetter(nextG)) {
                # pragma omp critical
                stack->push(nextG);
            } else {
                delete nextG;
            }
        }
        delete g;
    }
}


bool Solver::possiblyBetter(Graph * graph) const {
    if (SOLVER_DEBUG) { std::stringstream str; str << rank << "/" << omp_get_thread_num() << " checking possible solution: " << graph->getEdgeCount() << " ? " << incumbentObjective << std::endl; Logger::log(&str, rank); }
    return graph->getEdgeCount() > incumbentObjective ||
            (incumbent == nullptr && graph->getEdgeCount() == incumbentObjective);
}

bool Solver::isBipartite(Graph & graph) const {
    Bipartite *bp = new Bipartite(graph);
    bool result = bp->isBipartite;
    delete bp;
    return result;
}
