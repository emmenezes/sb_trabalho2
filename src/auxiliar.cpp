#include "../include/auxiliar.hpp"

void ConvertToX86(std::vector<int> matrix, fileData *file)
{
    int i = 0, op1, op2, data_size = 0;
    std::string *s_text = new std::string();
    std::string *s_data = new std::string();
    std::string *s_bss = new std::string();

    std::vector<instr> *instructions = new std::vector<instr>();
    std::vector<std::string> *text = new std::vector<std::string>();
    std::set<int> *labels = new std::set<int>();

    std::vector<varInt> *data = new std::vector<varInt>();
    std::map<int, int>::iterator it_data; // [posição na memoria, index]
    std::map<int, int> data_index;        // [posição na memoria, index]

    bool end = false;

    while (true)
    {
        instr instruction{i, ""};
        int op = matrix[i++];
        switch (op)
        {
        case 1: // ADD
            op1 = matrix[i++];
            instruction.text = "add \teax, [d" + std::to_string(op1) + "]\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 2: // SUB
            op1 = matrix[i++];
            instruction.text = "sub \teax, [d" + std::to_string(op1) + "]\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 3: //  MUL
            op1 = matrix[i++];
            break;
        case 4: // DIV
            op1 = matrix[i++];
            break;
        case 5: // JMP
            op1 = matrix[i++];
            instruction.text = "jmp \tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 6: // JMPN
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\n\tjl \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 7: // JMPP
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\n\tjg \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 8: // JPMZ
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\n\tjz \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 9: // COPY
            op1 = matrix[i++];
            op2 = matrix[i++];
            instruction.text = "mov \tebx, [d" + std::to_string(op2) + "]\n\tmov \t[d" + std::to_string(op1) + "], ebx\n";

            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }

            it_data = data_index.find(op2);
            if (it_data == data_index.end())
            {
                varInt var = {op2, false, matrix[op2]};
                data_index[op2] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 10: // LOAD
            op1 = matrix[i++];
            instruction.text = "mov \teax, [d" + std::to_string(op1) + "]\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 11: // STORE
            op1 = matrix[i++];
            instruction.text = "mov \tdword [d" + std::to_string(op1) + "], eax\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, true, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            else
            {
                (*data)[it_data->second].isVar = true;
            }
            break;
        case 12: // INPUT
            op1 = matrix[i++];
            break;
        case 13: // OUTPUT
            op1 = matrix[i++];
            break;
        case 14: // STOP
            end = true;
            instruction.text = "mov \teax, 1\n\tmov \tebx, 0\n\tint \t80h\n";
            break;
        case 15: // S_INPUT
            op1 = matrix[i++];
            op2 = matrix[i++];
            break;
        case 16: // S_OUTPUT
            op1 = matrix[i++];
            op2 = matrix[i++];
            break;
        }
        instructions->push_back(instruction);
        if (end)
            break;
    }
    CreateDataSection(data, s_data, s_bss);
    CreateTextSection(instructions, labels, s_text);
    SaveFile(s_text, s_data, s_bss, file);
}

void CreateDataSection(std::vector<varInt> *data, std::string *s_data, std::string *s_bss)
{
    for (int i = 0; i < data->size(); i++)
    {
        bool isVar = (*data)[i].isVar;
        std::string command;
        int val = (*data)[i].val;
        int index = (*data)[i].pos;
        if (isVar)
        {
            if (val)
            {
                command = 'd' + std::to_string(index) + "\t\tresdw " + std::to_string(val * 4) + '\n';
            }
            else
            {
                command = 'd' + std::to_string(index) + "\t\tresb 4" + '\n';
            }
            s_bss->append(command);
        }
        else
        {
            command = 'd' + std::to_string(index) + "\t\tdw " + std::to_string(val) + '\n';
            s_data->append(command);
        }
    }
}

void CreateTextSection(std::vector<instr> *instructions, std::set<int> *labels, std::string *s_text)
{
    int pos;
    std::string instruction;
    std::set<int>::iterator it;

    for (int i = 0; i < instructions->size(); i++)
    {
        pos = (*instructions)[i].pos;
        instruction = (*instructions)[i].text;
        it = (*labels).find(pos);
        if (it == (*labels).end())
        {
            s_text->append('\t' + instruction);
        }
        else
        {
            s_text->append('l' + std::to_string(pos) + ":\n\t" + instruction);
        }
    }
}

void SaveFile(std::string *s_text, std::string *s_data, std::string *s_bss, fileData *file)
{
    std::string program;

    program.append("section .data\n");
    program.append(*s_data);

    program.append("\nsection .bss\n");
    program.append(*s_bss);

    program.append("\nsection .text\nglobal_start\n_start:\n");
    program.append(*s_text);

    file->content = program;
}

std::vector<int> ConvertStringToIntVector(std::string text)
{
    std::string str;
    std::stringstream ss(text);
    std::vector<int> result;
    while (getline(ss, str, ' '))
    {
        result.push_back(stoi(str));
    }
    return result;
}
