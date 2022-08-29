#include "../include/auxiliar.hpp"

std::map<int, int> op_size_map = {
    // Instruções
    {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2},{8, 2},
    {9, 3}, {10, 2}, {11, 2}, {12, 2}, {13, 2}, {14, 1}, {15, 2}, {16,2}
};

void ConvertToX86(std::vector<int> matrix, fileData * file) {
    return;
}

void SaveFile(fileData * file) {
    return;
}

std::vector<int> ConvertStringToIntVector(std::string text){
    std::string str;
    std::stringstream ss(text);
    std::vector <int> result;
    while(getline(ss, str, ' ')){
        result.push_back(stoi(str));
    }
    return result;
}
