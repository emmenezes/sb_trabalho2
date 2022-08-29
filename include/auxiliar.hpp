#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct fileData {
    std::string name;
    std::string content;
};

void ConvertToX86(std::vector<int> matrix, fileData * file) ;

void SaveFile(fileData * file);

std::vector <int> ConvertStringToIntVector(std::string text);

#endif // !DATATYPES_H