#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
// #include "hash.h"  

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

int main() {
    std::string result = generateRandomString(12);
    std::cout << result << std::endl;
    
    return 0;
}