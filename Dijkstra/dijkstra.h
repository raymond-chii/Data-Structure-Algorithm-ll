#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include "hash.h"
#include "heap.h"
#include <limits>
#include <list>
#include <vector>

class graph{
    class vertex;
    class edge;

    public:
        graph();
        int loadGraph(const std::string &infile);
        void dijkstra(const std::string &source_vertex);
        bool exists(std:: string &vertex_id);
        void output(const std::string &outfile);
        static void decreaseKey(class vertex *temp, heap &pq);

    private:

        hashTable mapping;
        std::list<vertex> vertex_list;

        class vertex{
        public:
            std::string vertex_id;
            int distance = std::numeric_limits<int>::max();
            bool visited = false;
            std::list<edge> adjacent_list;
            std::vector<std::string> path;

        };
        class edge{
        public:
            vertex *destination_vertex;
            int cost;

        };

};


#endif //_DIJKSTRA_H