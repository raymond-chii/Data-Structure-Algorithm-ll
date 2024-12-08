#include <iostream>
#include <fstream>
#include <string>

std::string mergeStrings(std::string line1, std::string line2, std::string line3) {
    std::string result = line1 + " " + line2 + " " + line3;
    return result;
}

int main() {
    std::ifstream infile;
    std::ofstream outfile;

    std::string input_file, output_file;

    std::cout << "Enter name of input file: ";
    std::cin >> input_file;
    std::cout << "Enter name of output file: ";
    std::cin >> output_file;

    infile.open(input_file);
    if (!infile.is_open()) {
        std::cout << "Error opening input file.\n";
        return 1;
    }

    outfile.open(output_file);
    if (!outfile.is_open()) {
        std::cout << "Error opening output file.\n";
        infile.close();
        return 1;
    }

    std::string line1, line2, line3, line;
    while (std::getline(infile, line1) && std::getline(infile, line2) && std::getline(infile, line3)) {
        line = mergeStrings(line1, line2, line3);
        outfile << line << "\n";
    }

    infile.close();
    outfile.close();

    return 0;
}