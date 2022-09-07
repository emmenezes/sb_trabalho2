#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <set>

struct fileData
{
    std::string name;
    std::string content;
};

struct varInt
{
    int pos;
    bool isVar;
    int val;
};

struct varStr
{
    int pos;
    bool isVar;
    int size;
};

struct instr
{
    int pos;
    std::string text;
};

void ConvertToX86(std::vector<int> matrix, fileData *file);

void CreateDataSection(std::vector<int> matrix, std::vector<varInt> *data, std::vector<varStr> *data_str, std::string *s_data, std::string *s_bss);
void CreateTextSection(std::vector<instr> *instructions, std::set<int> *labels, std::string *s_text);

void SaveFile(std::string *s_text, std::string *s_data, std::string *s_bss, fileData *file);

std::vector<int> ConvertStringToIntVector(std::string text);

#endif // !DATATYPES_H