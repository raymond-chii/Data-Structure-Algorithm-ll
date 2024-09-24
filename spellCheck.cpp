#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <chrono>
#include "hash.h"

void loadDictionary(const std::string& filename, hashTable& dictionary);
void spellCheckDocument(const std::string& inputFilename, const std::string& outputFilename, hashTable& dictionary);
bool isValidWord(const std::string& word);

int main() {
    std::string dictionaryFile, documentFile, outputFile;
    hashTable dictionary(50000);

    // Prompt for file names
    std::cout << "Enter name of dictionary: ";
    std::cin >> dictionaryFile;

    // Measure dictionary loading time
    clock_t start = clock();
    loadDictionary(dictionaryFile, dictionary);
    clock_t end = clock();

    std::cout << "Total time (in seconds) to load dictionary: " 
              << std::fixed << std::setprecision(5) 
              << double(end - start) / CLOCKS_PER_SEC 
              << " seconds" << std::endl;

    std::cout << "Enter name of input file: ";
    std::cin >> documentFile;

    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;

    // Measure spell checking time 
    start = clock();
    spellCheckDocument(documentFile, outputFile, dictionary);
    end = clock();

    std::cout << "Total time (in seconds) to check document: " 
              << std::fixed << std::setprecision(5) 
              << double(end - start) / CLOCKS_PER_SEC 
              << " seconds" << std::endl;

    return 0;
}

void loadDictionary(const std::string& filename, hashTable& dictionary) {
    std::ifstream file(filename);
    std::string word;
    
    while (std::getline(file, word)) {
        // Convert word to lowercase
        for (char& c : word) {
            c = std::tolower(c);
        }
        
        // Insert valid words into the hash table
        if (isValidWord(word) && word.length() <= 20) {
            dictionary.insert(word);
        }
    }
}

void spellCheckDocument(const std::string& inputFilename, const std::string& outputFilename, hashTable& dictionary) {
    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);
    std::string line;
    int lineNumber = 0;

    while (std::getline(inputFile, line)) {
        lineNumber++;
        std::string word;
        bool inWord = false;

        for (size_t i = 0; i <= line.length(); i++) {
            char c = (i < line.length()) ? std::tolower(line[i]) : ' ';

            if (isValidWord(std::string(1, c))) {
                word += c;
                inWord = true;
            } else if (inWord) {
                if (word.length() > 20) {
                    outputFile << "Long word at line " << lineNumber << ", starts: " << word.substr(0, 20) << std::endl;
                } else if (!std::any_of(word.begin(), word.end(), ::isdigit) && !dictionary.contains(word)) {
                    outputFile << "Unknown word at line " << lineNumber << ": " << word << std::endl;
                }
                word.clear();
                inWord = false;
            }
        }
    }
}

bool isValidWord(const std::string& word) {
    for (char c : word) {
        if (!std::isalnum(c) && c != '-' && c != '\'') {
            return false;
        }
    }
    return true;
}