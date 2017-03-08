#ifndef MI_PDP_CPP_SOLVER_H
#define MI_PDP_CPP_SOLVER_H

#include "Graph.h"
#include <stack>

class Solver {
public:
    Solver(Graph &problem);

    virtual ~DFSSolver();

    Graph *getSolution() const;

    void solve();

private:
    std::stack<Graph> *stack;
    Graph *incumbent;
    int incumbentObjective;

    void setIncumbent(Graph &graph);
};


#endif
