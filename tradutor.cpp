#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "include/auxiliar.hpp"

using namespace std;

int main(int argc, char** argv){
    if (argc != 2){
        cout << "Quantidade de argumentos invalida, insira apenas o nome do arquivo com o codigo objeto\n" << endl;
        return 0;
    }

    fileData * input_file = new fileData{ .name=argv[1]};
    fileData * output_file = new fileData{ .name="saida.asm"};
    std::vector<int> matrix;
    
    ifstream ifs(argv[1]);
    stringstream buffer;
    buffer << ifs.rdbuf();
    input_file->content = buffer.str();

    matrix = ConvertStringToIntVector(input_file->content);
    ConvertToX86(matrix, output_file);

    ofstream ofs (output_file->name);
    ofs << output_file->content;
    ofs.close();

    cout << "Arquivo " << argv[1] << " convertido para o arquivo saida.asm" << endl;

}