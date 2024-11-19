#include "dijkstra.h"
#include <fstream>
#include <sstream>

graph::graph() = default;

int graph::loadGraph(const std::string &infile) {
    std::ifstream inputFile(infile);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    std::string line, source, destination, cost;
    bool b;

    while (getline(inputFile, line)) {
        std::istringstream stream(line);
        stream >> source >> destination >> cost;

        // Create or get source vertex
        vertex* source_vertex;
        if (!mapping.contains(source)) {
            vertex new_vertex;
            new_vertex.vertex_id = source;
            vertex_list.push_back(new_vertex);
            source_vertex = &vertex_list.back();
            mapping.insert(source, source_vertex);
        } else {
            source_vertex = static_cast<vertex*>(mapping.getPointer(source, &b));
        }

        // Create or get destination vertex
        vertex* destination_vertex;
        if (!mapping.contains(destination)) {
            vertex new_vertex;
            new_vertex.vertex_id = destination;
            vertex_list.push_back(new_vertex);
            destination_vertex = &vertex_list.back();
            mapping.insert(destination, destination_vertex);
        } else {
            destination_vertex = static_cast<vertex*>(mapping.getPointer(destination, &b));
        }

        // Create and add edge
        edge new_edge;
        new_edge.destination_vertex = destination_vertex;
        new_edge.cost = std::stoi(cost);
        source_vertex->adjacent_list.push_back(new_edge);
    }

    inputFile.close();
    return 0;
}

void graph::decreaseKey(vertex* current_vertex, heap& pqueue) {
    for (const auto& current_edge : current_vertex->adjacent_list) {
        vertex* adjacent_vertex = current_edge.destination_vertex;
        
        if (!adjacent_vertex->visited && 
            current_vertex->distance + current_edge.cost < adjacent_vertex->distance &&
            current_vertex->distance != std::numeric_limits<int>::max()) {
            
            adjacent_vertex->distance = current_vertex->distance + current_edge.cost;
            adjacent_vertex->path = current_vertex->path;
            adjacent_vertex->path.push_back(adjacent_vertex->vertex_id);
            pqueue.setKey(adjacent_vertex->vertex_id, adjacent_vertex->distance);
        }
    }
}

void graph::dijkstra(const std::string &source_vertex){
    bool b;

    vertex *source = static_cast<vertex*>(mapping.getPointer(source_vertex, &b));
    heap pq(vertex_list.size());

    (*source).distance = 0;
    (*source).path.push_back(source_vertex);

    for (vertex& v: vertex_list) {
        pq.insert(v.vertex_id, v.distance, &v);
    }

    vertex *current_vertex;
    while (!pq.deleteMin(nullptr, nullptr, &current_vertex)) {
        (*current_vertex).visited = true;
        decreaseKey(current_vertex, pq);
    }
}

bool graph::exists(std:: string &vertex_id){
    return mapping.contains(vertex_id);
}

void graph::output(const std::string &outfile){
    std::ofstream outputFile(outfile);
    
    for (vertex& v : vertex_list){
        outputFile << v.vertex_id << ": ";

        if(v.path.empty()){
            outputFile << "NO PATH" << std::endl;
        }
        else{
            outputFile << v.distance << " [";

            for (int i = 0; i < v.path.size() - 1; i++){
                outputFile << v.path[i] << ", ";
            }

            outputFile << v.path.back();
            outputFile << "]";

            if (v.vertex_id != vertex_list.back().vertex_id){
                outputFile<<std::endl;
            }

        }    

    }

    outputFile.close();

}
