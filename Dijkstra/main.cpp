#include <iostream>
#include "dijkstra.h"
#include <ctime>

int main() {
    std::string infile;
    graph inGraph;
    clock_t start, end;

    std::cout << "Enter name of graph file: ";
    std::cin >> infile;
    
    // Load graph
    if (inGraph.loadGraph(infile)) {
        return 1;  // Exit if file loading fails
    }

    std::string source;
    std::string outfile;

    // Get valid starting vertex
    std::cout << "Enter name of starting vertex: ";
    std::cin >> source;
    while (!inGraph.exists(source)) {
        std::cout << "Enter name of starting vertex: ";
        std::cin >> source;
    }

    
    start = clock();
    inGraph.dijkstra(source);
    end = clock();

    double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " 
              << cpu_time << std::endl;

    std::cout << "Enter name of output file: ";
    std::cin >> outfile;
    inGraph.output(outfile);

    return 0;
}
