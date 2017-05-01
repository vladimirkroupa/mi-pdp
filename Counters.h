#ifndef MI_PDP_CPP_COUNTERS_H
#define MI_PDP_CPP_COUNTERS_H

class Counters {

public:
    Counters();
    void graphCreated();
    void graphDestroyed();
    int getLiveGraphs();
    void printCounters();
private:
    int graphs;
};

#endif //MI_PDP_CPP_COUNTERS_H
