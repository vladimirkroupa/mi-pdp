#ifndef MI_PDP_CPP_SOLVER_H
#define MI_PDP_CPP_SOLVER_H

#include "Graph.h"
#include "Counters.h"
#include <deque>

class Solver {
public:
    Solver(Graph *problem, int threads);

    virtual ~Solver();

    Graph *getSolution() const;

    void solve();

private:
    std::vector<Graph *> _deque;
    Graph * incumbent;
    int incumbentObjective;
    int printSkip = 0;
    int threads;

    void doSolve(std::vector<Graph *> *deque, int statesToSolve);
    void solveState(std::vector<Graph *> * deque, Graph *g);

    bool possiblyBetter(Graph * graph) const;
    bool isBipartite(Graph & graph) const;
    void setIncumbent(Graph * graph);

    Counters counters;
};


#endif
