#include "Counters.h"

Counters::Counters() {
    graphs = 1;
}

void Counters::graphCreated() {
    graphs++;
    printCounters();

}

void Counters::graphDestroyed() {
    graphs--;
    printCounters();
}

int Counters::getLiveGraphs() {
    return graphs;
}

void Counters::printCounters() {
    //std::cout << counters.getLiveGraphs() << std::endl;
}
