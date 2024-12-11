#include <iostream>
#include <fstream>
#include <string>

bool Matrix[1000][1000] = {false};

std::string mergeStrings(std::string line1, std::string line2, std::string line3) {

    memset(Matrix, false, sizeof(Matrix));

    int len1 = line1.length();
    int len2 = line2.length();
    int len3 = line3.length();

    if((len1 + len2) >len3){
        return "*** NOT A MERGE ***";
    }

    for (int i = 0; i < len1; i++){
        for (int j = 0; j < len2; j++){
            if(line1[i] == line2[j]){

                Matrix[i][j] = true;

            } else if (i == 0 && line2[j-1] == line3[j-1]){
                
                Matrix[i][j] = Matrix[i][j-1];
                
            } else if (j == 0 && line1[i-1] == line3[i-1]){
                
                Matrix[i][j] = Matrix[i-1][j];

            } else {
                if(line1[i-1] == line3[i+j-1] && line2[j-1] == line3[i+j-1]){

                    Matrix[i][j] = Matrix[i-1][j] || Matrix[i][j-1];

                } else if(line1[i-1] == line3[i+j-1] && line2[j-1] != line3[i+j-1]){
                    
                    Matrix[i][j] = Matrix[i-1][j];

                } else if(line1[i-1] != line3[i+j-1] && line2[j-1] == line3[i+j-1]){

                    Matrix[i][j] = Matrix[i][j-1];

                }
            }
        }
    }

    if (Matrix[len1][len2]) {
        std::string result;
        int i = len1, j = len2;

        while (i > 0 || j > 0) {
            if (i > 0 && Matrix[i-1][j] && line1[i-1] == line3[i+j-1]) {
                result += std::toupper(line1[i-1]);
                i--;
            } else if (j > 0 && Matrix[i][j-1] && line2[j-1] == line3[i+j-1]) {
                result += line2[j-1];
                j--;
            }
        }

        std::reverse(result.begin(), result.end());
        return result;
    } else {
        return "*** NOT A MERGE ***";
    }
        

    
    


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