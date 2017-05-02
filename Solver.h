#ifndef MI_PDP_CPP_SOLVER_H
#define MI_PDP_CPP_SOLVER_H

#include "Graph.h"
#include <stack>

#define SOLVER_DEBUG false

class Solver {
public:
    Solver(Graph &problem, int threads, int rank);

    virtual ~Solver();

    Graph *getSolution() const;

    void solve();

private:
    std::stack<Graph *> _stack;
    Graph * incumbent;
    int incumbentObjective;
    int printSkip = 0;
    int threads;

    void doSolve(std::stack<Graph *> * stack);
    void solveState(std::stack<Graph *> * stack, Graph *g);

    bool possiblyBetter(Graph * graph) const;
    bool isBipartite(Graph & graph) const;
    void setIncumbent(Graph * graph);

    int rank;
};


#endif
