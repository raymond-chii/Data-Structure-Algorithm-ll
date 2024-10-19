#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include "hash.h"
#include <string>

std::string generateRandomString(int length) {
    static const std::string CHAR_SET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-'";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, CHAR_SET.size() - 1);

    std::string result;
    result.reserve(length); // to avoid reallocation 
    for (int i = 0; i < length; ++i) { 
        result += CHAR_SET[dist(gen)];
    }    
    
    return result;
}

double benchmarkLoadDict(std:: string &filename, hashTable &dictionary, int generateWords = 10000) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream inputFile(filename);
    std::string word;
    int wordCount = 0;
    while (std::getline(inputFile, word) && wordCount < 100000) {  // Limit to prevent excessive loading time
        for (char& c : word) {
            c = std::tolower(c);
        }
        if (word.length() <= 20) {
            dictionary.insert(word);
            ++wordCount;
        }
    }

    for (int i = 0; i < generateWords; ++i) {
        dictionary.insert(generateRandomString(rand() % 20 + 1));
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start; 
    std::cout << "Loaded" << wordCount << " real words and " << generateWords << "synthetic words" << std::endl;

    return diff.count();

}

int main() {
    hashTable dictionary(150000);
    std::string dictionaryFile = "benchmarkData/cleaned_dictionary.txt";
    
    double loadTime = benchmarkLoadDict(dictionaryFile, dictionary);
    std::cout << "Dictionary load time: " << loadTime << " seconds" << std::endl;
    
    return 0;
}