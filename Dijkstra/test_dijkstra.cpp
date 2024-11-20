#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <set>
#include <iomanip>
#include "dijkstra.h"

// Utility class to generate complex graph patterns
class GraphGenerator {
private:
    std::mt19937 rng;
    
public:
    GraphGenerator() : rng(std::random_device{}()) {}
    
    // Generate a complete graph with n vertices
    std::vector<std::string> generateComplete(int n, int maxWeight = 100) {
        std::vector<std::string> edges;
        std::uniform_int_distribution<int> dist(1, maxWeight);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    edges.push_back("v" + std::to_string(i) + 
                                  " v" + std::to_string(j) + 
                                  " " + std::to_string(dist(rng)));
                }
            }
        }
        return edges;
    }
    
    // Generate a grid graph with dimensions n x m
    std::vector<std::string> generateGrid(int n, int m, int maxWeight = 100) {
        std::vector<std::string> edges;
        std::uniform_int_distribution<int> dist(1, maxWeight);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (j + 1 < m) {  // Horizontal edges
                    edges.push_back("v" + std::to_string(i) + "_" + std::to_string(j) +
                                  " v" + std::to_string(i) + "_" + std::to_string(j+1) +
                                  " " + std::to_string(dist(rng)));
                }
                if (i + 1 < n) {  // Vertical edges
                    edges.push_back("v" + std::to_string(i) + "_" + std::to_string(j) +
                                  " v" + std::to_string(i+1) + "_" + std::to_string(j) +
                                  " " + std::to_string(dist(rng)));
                }
            }
        }
        return edges;
    }
    
    // Generate a random graph with specified density
    std::vector<std::string> generateRandom(int n, double density, int maxWeight = 100) {
        std::vector<std::string> edges;
        std::uniform_int_distribution<int> weight_dist(1, maxWeight);
        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j && prob_dist(rng) < density) {
                    edges.push_back("v" + std::to_string(i) + 
                                  " v" + std::to_string(j) + 
                                  " " + std::to_string(weight_dist(rng)));
                }
            }
        }
        return edges;
    }
};

// Test result structure
struct TestResult {
    std::string name;
    bool passed;
    double time;
    std::string details;
};

// Helper functions
void writeGraphToFile(const std::string& filename, const std::vector<std::string>& edges) {
    std::ofstream file(filename);
    for (const auto& edge : edges) {
        file << edge << "\n";
    }
}

std::map<std::string, std::pair<int, std::vector<std::string>>> parseOutput(const std::string& filename) {
    std::map<std::string, std::pair<int, std::vector<std::string>>> results;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        std::string vertex = line.substr(0, line.find(':'));
        std::string rest = line.substr(line.find(':') + 2);
        
        if (rest == "NO PATH") {
            results[vertex] = {-1, {}};
            continue;
        }
        
        int distance = std::stoi(rest.substr(0, rest.find(' ')));
        std::string path = rest.substr(rest.find('[') + 1);
        path = path.substr(0, path.length() - 1);
        
        std::vector<std::string> pathVec;
        size_t pos = 0;
        while ((pos = path.find(", ")) != std::string::npos) {
            pathVec.push_back(path.substr(0, pos));
            path.erase(0, pos + 2);
        }
        pathVec.push_back(path);
        
        results[vertex] = {distance, pathVec};
    }
    return results;
}

// Complex test cases
std::vector<TestResult> runComplexTests() {
    std::vector<TestResult> results;
    GraphGenerator generator;
    
    // Test 1: Large Complete Graph (50 vertices)
    {
        TestResult result{"Large Complete Graph (50 vertices)", false, 0.0, ""};
        try {
            auto edges = generator.generateComplete(50);
            writeGraphToFile("complete50.txt", edges);
            
            auto start = std::chrono::high_resolution_clock::now();
            graph g;
            g.loadGraph("complete50.txt");
            g.dijkstra("v0");
            g.output("complete50_out.txt");
            auto end = std::chrono::high_resolution_clock::now();
            result.time = std::chrono::duration<double>(end - start).count();
            
            auto paths = parseOutput("complete50_out.txt");
            assert(paths.size() == 50);  // Should have paths to all vertices
            result.passed = true;
        } catch (const std::exception& e) {
            result.details = e.what();
        }
        results.push_back(result);
    }
    
    // Test 2: Large Grid Graph (30x30)
    {
        TestResult result{"Large Grid Graph (30x30)", false, 0.0, ""};
        try {
            auto edges = generator.generateGrid(30, 30);
            writeGraphToFile("grid30x30.txt", edges);
            
            auto start = std::chrono::high_resolution_clock::now();
            graph g;
            g.loadGraph("grid30x30.txt");
            g.dijkstra("v0_0");
            g.output("grid30x30_out.txt");
            auto end = std::chrono::high_resolution_clock::now();
            result.time = std::chrono::duration<double>(end - start).count();
            
            auto paths = parseOutput("grid30x30_out.txt");
            assert(paths["v29_29"].first != -1);  // Should be able to reach opposite corner
            result.passed = true;
        } catch (const std::exception& e) {
            result.details = e.what();
        }
        results.push_back(result);
    }
    
    // Test 3: Dense Random Graph (200 vertices, 80% density)
    {
        TestResult result{"Dense Random Graph (200 vertices)", false, 0.0, ""};
        try {
            auto edges = generator.generateRandom(200, 0.8);
            writeGraphToFile("dense200.txt", edges);
            
            auto start = std::chrono::high_resolution_clock::now();
            graph g;
            g.loadGraph("dense200.txt");
            g.dijkstra("v0");
            g.output("dense200_out.txt");
            auto end = std::chrono::high_resolution_clock::now();
            result.time = std::chrono::duration<double>(end - start).count();
            
            auto paths = parseOutput("dense200_out.txt");
            result.passed = true;
        } catch (const std::exception& e) {
            result.details = e.what();
        }
        results.push_back(result);
    }
    
    // Test 4: Mixed Graph Patterns
    {
        TestResult result{"Mixed Graph Patterns", false, 0.0, ""};
        try {
            std::vector<std::string> edges;
            // Add a complete subgraph
            auto complete = generator.generateComplete(10);
            edges.insert(edges.end(), complete.begin(), complete.end());
            
            // Add a grid subgraph
            auto grid = generator.generateGrid(5, 5);
            for (const auto& edge : grid) {
                std::string modified = edge;
                modified.insert(1, "g");  // Prefix grid vertices with 'g'
                edges.push_back(modified);
            }
            
            // Add connections between subgraphs
            edges.push_back("v0 vg0_0 50");
            edges.push_back("v9 vg4_4 50");
            
            writeGraphToFile("mixed.txt", edges);
            
            auto start = std::chrono::high_resolution_clock::now();
            graph g;
            g.loadGraph("mixed.txt");
            g.dijkstra("v0");
            g.output("mixed_out.txt");
            auto end = std::chrono::high_resolution_clock::now();
            result.time = std::chrono::duration<double>(end - start).count();
            
            auto paths = parseOutput("mixed_out.txt");
            assert(paths["vg4_4"].first != -1);  // Should be able to reach grid subgraph
            result.passed = true;
        } catch (const std::exception& e) {
            result.details = e.what();
        }
        results.push_back(result);
    }
    
    // Test 5: Maximum Weight Paths
    {
        TestResult result{"Maximum Weight Paths", false, 0.0, ""};
        try {
            std::vector<std::string> edges;
            for (int i = 0; i < 100; i++) {
                edges.push_back("v" + std::to_string(i) + 
                              " v" + std::to_string(i+1) + 
                              " 999999");
            }
            // Add some shortcuts with very high weights
            for (int i = 0; i < 90; i += 10) {
                edges.push_back("v" + std::to_string(i) + 
                              " v" + std::to_string(i+10) + 
                              " 9999980");
            }
            
            writeGraphToFile("maxweight.txt", edges);
            
            auto start = std::chrono::high_resolution_clock::now();
            graph g;
            g.loadGraph("maxweight.txt");
            g.dijkstra("v0");
            g.output("maxweight_out.txt");
            auto end = std::chrono::high_resolution_clock::now();
            result.time = std::chrono::duration<double>(end - start).count();
            
            auto paths = parseOutput("maxweight_out.txt");
            assert(paths["v100"].first < 100000000);  // Should take shorter path
            result.passed = true;
        } catch (const std::exception& e) {
            result.details = e.what();
        }
        results.push_back(result);
    }
    
    return results;
}

int main() {
    std::cout << "Running complex Dijkstra tests...\n\n";
    
    auto results = runComplexTests();
    
    // Print results in a formatted table
    std::cout << std::setw(30) << std::left << "Test Name" 
              << std::setw(10) << "Status" 
              << std::setw(15) << "Time (s)" 
              << "Details\n";
    std::cout << std::string(70, '-') << "\n";
    
    bool allPassed = true;
    for (const auto& result : results) {
        std::cout << std::setw(30) << std::left << result.name
                  << std::setw(10) << (result.passed ? "PASSED" : "FAILED")
                  << std::setw(15) << std::fixed << std::setprecision(6) << result.time;
        if (!result.passed) {
            std::cout << result.details;
            allPassed = false;
        }
        std::cout << "\n";
    }
    
    std::cout << "\nSummary:\n";
    std::cout << "Total tests: " << results.size() << "\n";
    std::cout << "Passed: " << std::count_if(results.begin(), results.end(), 
                                           [](const TestResult& r) { return r.passed; }) << "\n";
    std::cout << "Failed: " << std::count_if(results.begin(), results.end(), 
                                           [](const TestResult& r) { return !r.passed; }) << "\n";
    
    return allPassed ? 0 : 1;
}