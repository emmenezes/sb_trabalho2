#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "include/auxiliar.hpp"

using namespace std;

int main(int argc, char** argv){
    if (argc != 1){
        cout << "Quantidade de argumentos invalida, insira apenas o nome do arquivo com o codigo objeto\n" << endl;
        return 0;
    }

    fileData * input_file = new fileData{ .name=argv[1]};
    fileData * output_file = new fileData{ .name="saida.asm"};
    std::vector<std::vector<int>> * matrix;
    
    ifstream ifs(argv[2]);
    stringstream buffer;
    buffer << ifs.rdbuf();
    input_file->content = buffer.str();

    ConvertFileToMatrix(input_file, matrix);
    ConvertToX86(matrix, output_file);
    SaveFile(output_file);

    cout << "Arquivo " << argv[1] << " convertido para o arquivo saida.asm" << endl;

}