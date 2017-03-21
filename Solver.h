#ifndef MI_PDP_CPP_SOLVER_H
#define MI_PDP_CPP_SOLVER_H

#include "Graph.h"
#include <stack>

class Solver {
public:
    Solver(Graph &problem);

    virtual ~Solver();

    Graph *getSolution() const;

    void solve();

private:
    std::stack<Graph *> _stack;
    Graph * incumbent;
    int incumbentObjective;

    void doSolve(std::stack<Graph *> * stack);

    void solveState(std::stack<Graph *> * stack, Graph * graph);

    bool isBipartite(Graph & graph) const;
    void setIncumbent(Graph * graph);
};


#endif
