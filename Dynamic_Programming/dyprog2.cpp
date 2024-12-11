#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

bool Matrix[1001][1001];

std::string mergeStrings(std::string line1, std::string line2, std::string line3) {
    memset(Matrix, false, sizeof(Matrix));

    int len1 = line1.length();
    int len2 = line2.length();
    int len3 = line3.length();

    if (len1 + len2 != len3) {
        return "*** NOT A MERGE ***";
    }

    Matrix[0][0] = true;

    // Initialize first row
    for (int j = 1; j <= len2; j++) {
        Matrix[0][j] = Matrix[0][j - 1] && (line2[j - 1] == line3[j - 1]);
    }

    // Initialize first column
    for (int i = 1; i <= len1; i++) {
        Matrix[i][0] = Matrix[i - 1][0] && (line1[i - 1] == line3[i - 1]);
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            char c = line3[i + j - 1];
            if (line1[i - 1] == c && line2[j - 1] != c) {
                Matrix[i][j] = Matrix[i - 1][j];
            }
            else if (line1[i - 1] != c && line2[j - 1] == c) {
                Matrix[i][j] = Matrix[i][j - 1];
            }
            else if (line1[i - 1] == c && line2[j - 1] == c) {
                Matrix[i][j] = Matrix[i - 1][j] || Matrix[i][j - 1];
            }
        }
    }

    if (!Matrix[len1][len2]) {
        return "*** NOT A MERGE ***";
    }

    // Modified reconstruction to ensure leftmost placement of uppercase letters
    std::string result = line3;
    int j = len2;
    
    for (int i = len1; i > 0; i--) {
        while (true) {
            if (Matrix[i][j] && (j == 0 || !Matrix[i][j - 1])) {
                result[i + j - 1] -= 32;  // Convert to uppercase
                break;
            }
            j--;
        }
    }
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

    std::string line1, line2, line3;
    while (std::getline(infile, line1) && std::getline(infile, line2) && std::getline(infile, line3)) {
        outfile << mergeStrings(line1, line2, line3) << "\n";
    }

    infile.close();
    outfile.close();

    return 0;
}