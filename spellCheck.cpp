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
    std::string dictionaryFile, inputFile, outputFile;
    hashTable dictionary(50000);

    // Prompt for dictionary file
    std::cout << "Enter name of dictionary: ";
    std::cin >> dictionaryFile;

    // Load dictionary and measure time
    auto start = std::chrono::high_resolution_clock::now();
    loadDictionary(dictionaryFile, dictionary);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total time (in seconds) to load dictionary: " << diff.count() << std::endl;

    // Prompt for input and output files
    std::cout << "Enter name of input file: ";
    std::cin >> inputFile;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;

    // Spell check the document and measure time
    start = std::chrono::high_resolution_clock::now();
    spellCheckDocument(inputFile, outputFile, dictionary);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Total time (in seconds) to check document: " << diff.count() << std::endl;

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
                // Word ended, check it
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