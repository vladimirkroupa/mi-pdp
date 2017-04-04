#ifndef MI_PDP_CPP_SOLVER_H
#define MI_PDP_CPP_SOLVER_H

#include "Graph.h"
#include <deque>

class Solver {
public:
    Solver(Graph &problem, int threads);

    virtual ~Solver();

    Graph *getSolution() const;

    void solve();

private:
    std::deque<Graph *> _deque1;
    std::deque<Graph *> _deque2;
    Graph * incumbent;
    int incumbentObjective;
    int printSkip = 0;
    int threads;

    void doSolve(std::deque<Graph *> *workDeque, std::deque<Graph *> *auxDeque);
    void solveState(std::deque<Graph *> * auxDeque, Graph *g);

    bool isBipartite(Graph & graph) const;
    void setIncumbent(Graph * graph);
};


#endif
