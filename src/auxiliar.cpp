#include "../include/auxiliar.hpp"

void ConvertToX86(std::vector<int> matrix, fileData *file)
{
    int i = 0, op1, op2, data_size = 0, data_str_size = 0;
    std::string *s_text = new std::string();
    std::string *s_data = new std::string();
    std::string *s_bss = new std::string();

    std::vector<instr> *instructions = new std::vector<instr>();
    std::vector<std::string> *text = new std::vector<std::string>();
    std::set<int> *labels = new std::set<int>();

    std::vector<varInt> *data = new std::vector<varInt>();
    std::map<int, int>::iterator it_data; // [posição na memoria, index]
    std::map<int, int> data_index;        // [posição na memoria, index]

    std::vector<varStr> *data_str = new std::vector<varStr>();
    std::map<int, int>::iterator it_data_str; // [posição na memoria, index]
    std::map<int, int> data_index_str;        // [posição na memoria, index]

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
            instruction.text = "mov \tebx, [d" + std::to_string(op1) + "]\
                            \n\timul \tebx\
                            \n\tjnc \tl" + std::to_string(i) + "\
                            \n\tmov \teax,1\
                            \n\tmov \tebx, 0\
                            \n\tint \t80h\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            labels->insert(i);
            break;
        case 4: // DIV
            op1 = matrix[i++];
            instruction.text = "mov \tebx, [d" + std::to_string(op1) + "]\
                            \n\tcdq\
                            \n\tidiv \tebx\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 5: // JMP
            op1 = matrix[i++];
            instruction.text = "jmp \tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 6: // JMPN
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\
                            \n\tjl \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 7: // JMPP
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\
                            \n\tjg \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 8: // JPMZ
            op1 = matrix[i++];
            instruction.text = "cmp \teax, 0\
                            \n\tjz \t\tl" + std::to_string(op1) + "\n";
            labels->insert(op1);
            break;
        case 9: // COPY
            op1 = matrix[i++];
            op2 = matrix[i++];
            instruction.text = "mov \tebx, [d" + std::to_string(op1) + "]\
                            \n\tmov \t[d" + std::to_string(op2) + "], ebx\n";

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
            instruction.text = "push eax\
                            \n\tpush d" + std::to_string(op1) + "\
                            \n\tcall INPUT\
                            \n\rpop eax\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 13: // OUTPUT
            op1 = matrix[i++];
            instruction.text = "push eax\
                            \n\tpush dword [d" + std::to_string(op1) + "]\
                            \n\tcall OUTPUT\
                            \n\rpop eax\n";
            it_data = data_index.find(op1);
            if (it_data == data_index.end())
            {
                varInt var = {op1, false, matrix[op1]};
                data_index[op1] = data_size;
                data->push_back(var);
                data_size++;
            }
            break;
        case 14: // STOP
            end = true;
            instruction.text = "mov \teax, 1\
                            \n\tmov \tebx, 0\
                            \n\tint \t80h\n";
            break;
        case 15: // S_INPUT
            op1 = matrix[i++];
            op2 = matrix[i++];
            instruction.text = "push eax\
                            \n\tpush \td" + std::to_string(op1) + "\
                            \n\tpush \tword " + std::to_string(op2) + "\
                            \n\tcall S_INPUT\n\
                            \n\tpop eax\n";
            it_data_str = data_index_str.find(op1);
            if (it_data_str == data_index_str.end())
            {
                varStr var = {op1, true, op2};
                data_index_str[op1] = data_size;
                data_str->push_back(var);
                data_str_size++;
            }
            break;
        case 16: // S_OUTPUT
            op1 = matrix[i++];
            op2 = matrix[i++];
            instruction.text = "push eax\
                            \n\tpush \td" + std::to_string(op1) + "\
                            \n\tpush \tword " + std::to_string(op2) + "\
                            \n\tcall S_OUTPUT\n\
                            \n\tpop eax\n";
            it_data_str = data_index_str.find(op1);
            if (it_data_str == data_index_str.end())
            {
                varStr var = {op1, true, op2};
                data_index_str[op1] = data_size;
                data_str->push_back(var);
                data_str_size++;
            }
            break;
        }
        instructions->push_back(instruction);
        if (end)
            break;
    }
    CreateDataSection(matrix, data, data_str, s_data, s_bss);
    CreateTextSection(instructions, labels, s_text);
    SaveFile(s_text, s_data, s_bss, file);
}

void CreateDataSection(std::vector<int> matrix, std::vector<varInt> *data, std::vector<varStr> *data_str, std::string *s_data, std::string *s_bss)
{
    s_data->append( "msg1\t\tdb\t'Foram lidos/escritos '\
                    \nMSG1SIZE\tEQU\t$-msg1\
                    \nmsg2\t\tdb\t' bytes.', 0Dh, 0Ah\
                    \nMSG2SIZE\tEQU\t$-msg2\
                    \nnwln\t\tdb\t0Dh, 0Ah\
                    \nNWLNSIZE\tEQU $-nwln\n\n");

    for (int i = 0; i < data->size(); i++)
    {
        bool isVar = (*data)[i].isVar;
        std::string command;
        int val = (*data)[i].val;
        int index = (*data)[i].pos;
        if (isVar)
        {
            command = 'd' + std::to_string(index) + "\t\tresb 4" + '\n';
            s_bss->append(command);
        }
        else
        {
            command = 'd' + std::to_string(index) + "\t\tdd " + std::to_string(val) + '\n';
            s_data->append(command);
        }
    }

    for (int i = 0; i < data_str->size(); i++)
    {
        std::string command;
        int index = (*data_str)[i].pos;
        bool isVar = (*data_str)[i].isVar;
        int size = (*data_str)[i].size;
        if (isVar)
        {
            command = 'd' + std::to_string(index) + "\t\tresb" + std::to_string(size) + '\n';
            s_bss->append(command);
        }
        else
        {
            std::string text;
            for (int j = 0; j < size; j++) {
                text += (char)matrix[index+j];
            }
            command = "db \"" + text + "\", 0dH, 0aH\n";
            s_data->append(command);
        }
    }
}

void CreateTextSection(std::vector<instr> *instructions, std::set<int> *labels, std::string *s_text)
{
    int pos;
    std::string instruction;
    std::set<int>::iterator it;

    // Funções auxiliares
    s_text->append( "MSGOUT:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
                \n\tpush ebx\
                \n\tpush ecx\
                \n\tpush edx\
                \n\tpush esi\
            \n\t\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tmov ecx, msg1\
                \n\tmov edx, MSG1SIZE\
                \n\tint 80h\
            \n\t\
                \n\tmov eax, [ebp+8]\
                \n\tsub esp, 12\
                \n\tmov ecx, esp\
                \n\tpush eax\
                \n\tpush ecx\
                \n\tcall CVTDCHX\
                \n\tadd esp, 12\
            \n\t\
                \n\tmov edx, eax\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tint 80h\
            \n\t\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tmov ecx, msg2\
                \n\tmov edx, MSG2SIZE\
                \n\tint 80h\
            \n\t\
                \n\tpop esi\
                \n\tpop edx\
                \n\tpop ecx\
                \n\tpop ebx\
                \n\tpop ebp\
                \n\tret 4\
            \n\t\
            \nCVTDCHX:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
                \n\tpush ebx\
                \n\tpush ecx\
                \n\tpush edx\
                \n\tpush esi\
            \n\t\
                \n\tmov eax, [ebp+12]   ; valor\
                \n\tmov esi, [ebp+8]    ; ponteiro\
                \n\tmov byte [esi], 0x0\
                \n\tpush esi\
                \n\tcmp eax, 0\
                \n\tjge CVTS \
            \n\t\
                \n\tpop ebx\
                \n\tmov byte [esi], 0x2d\
                \n\tinc esi\
                \n\tpush esi\
                \n\tmov byte [esi], 0x0\
                \n\tneg eax\
            \n\t\
            \nCVTS:\
                \n\tcdq\
                \n\tmov ebx, 10\
                \n\tidiv ebx\
                \n\tadd dl, 0x30\
                \n\t\
            \nCVTS_LOOP:\
                \n\tmov cl, [esi]\
                \n\tcmp cl, 0\
                \n\tjz CVTS_FIM\
                \n\tmov byte [esi], dl\
                \n\tmov dl, cl\
                \n\tinc esi\
                \n\tjmp CVTS_LOOP\
            \n\t\
            \nCVTS_FIM:\
                \n\tmov byte [esi], dl\
                \n\tinc esi\
                \n\tmov byte [esi], 0x0\
                \n\tpop esi\
                \n\tpush esi\
            \n\t\
                \n\tcmp eax, 0\
                \n\tjz CVTDCHX_CNT\
                \n\tjmp CVTS\
            \n\t\
            \nCVTDCHX_CNT:\
                \n\tpop esi\
                \n\tmov eax, 0\
                \n\tmov esi, [ebp+8]\
            \n\t\
            \nCVTDCHX_CNT_LOOP:\
                \n\tcmp byte [esi], 0x0\
                \n\tjz CVTDCHX_FIM\
                \n\tinc eax\
                \n\tinc esi\
                \n\tjmp CVTDCHX_CNT_LOOP\
            \n\t\
            \nCVTDCHX_FIM:\
                \n\tpop esi\
                \n\tpop edx\
                \n\tpop ecx\
                \n\tpop ebx\
                \n\tpop ebp\
                \n\tret 8\n\n");

    // Função de s_input
    s_text->append( "S_INPUT:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
                \n\tsub edx, edx\
                \n\t\
                \n\tmov eax, 3\
                \n\tmov ebx, 0\
                \n\tmov ecx, [ebp+10]\
                \n\tmov dx, [ebp+8]\
                \n\tint 80h\
                \n\t\
                \n\tpush eax\
                \n\tpush eax\
                \n\tcall MSGOUT\
                \n\tpop eax\
                \n\t\
                \n\tpop ebp\
                \n\tret 10\n\n");

    // Função de s_output
    s_text->append("S_OUTPUT:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
                \n\tsub edx, edx\
            \n\t\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tmov ecx, [ebp+10]\
                \n\tmov dx, [ebp+8]\
                \n\tint 80h\
            \n\t\
                \n\tpush eax\
                \n\tpush eax\
                \n\tcall MSGOUT\
                \n\tpop eax\
            \n\t\
                \n\tpop ebp\
                \n\tret 10\n\n");
    
    // Função de input
    s_text->append("INPUT:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
            \n\t\
                \n\tsub esp, 12\
                \n\tmov ecx, esp\
            \n\t\
                \n\tmov eax, 3\
                \n\tmov ebx, 0\
                \n\tmov edx, 12\
                \n\tint 80h\
            \n\t\
                \n\tpush eax\
                \n\tpush eax\
                \n\tcall MSGOUT\
                \n\tpop eax\
                \n\tpush eax\
            \n\t\
                \n\tsub ebx, ebx\
                \n\tmov edx, ecx\
                \n\tmov ecx, eax\
                \n\tmov esi, 0\
                \n\tmov eax, 0\
            \n\t\
                \n\tmov bl, [edx]\
                \n\tdec ecx\
                \n\tcmp bl, 0x2d\
                \n\tjne INPUT_COUNT\
                \n\tmov esi, 1\
                \n\tinc edx\
                \n\tdec ecx\
            \n\t\
            \nINPUT_COUNT:\
                \n\tmov bl, [edx]\
                \n\tinc edx\
                \n\tsub bl, 0x30\
            \n\t\
                \n\tpush ebx\
                \n\tshl eax, 1\
                \n\tmov ebx, eax\
                \n\tshl eax, 2\
                \n\tadd eax, ebx\
                \n\tpop ebx\
                \n\tadd eax, ebx\
                \n\tloop INPUT_COUNT\
            \n\t\
                \n\tcmp esi, 0\
                \n\tjz INPUT_FIM\
                \n\tneg eax\
            \n\t\
            \nINPUT_FIM:\
                \n\tmov ebx, [ebp+8]\
                \n\tmov [ebx], eax\
                \n\tpop eax\
                \n\tadd esp, 12\
                \n\tpop ebp\
                \n\tret 4\n\n");

    // Função de output
    s_text->append("OUTPUT:\
                \n\tpush ebp\
                \n\tmov ebp, esp\
            \n\t\
                \n\tmov eax, [ebp+8]\
            \n\t\
                \n\tsub esp, 12\
                \n\tmov ecx, esp\
                \n\tpush eax\
                \n\tpush ecx\
                \n\tcall CVTDCHX\
                \n\tpush eax\
                \n\tpush eax\
            \n\t\
                \n\tmov edx, eax\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tint 80h\
            \n\t\
                \n\tmov eax, 4\
                \n\tmov ebx, 1\
                \n\tmov ecx, nwln\
                \n\tmov edx, NWLNSIZE\
                \n\tint 80h\
            \n\t\
                \n\tcall MSGOUT\
            \n\t\
                \n\tpop eax\
                \n\tadd esp, 12\
                \n\tpop ebp\
                \n\tret 4\n\n");

    // Label de _start
    s_text->append("\n_start:\n");

    // Instruções do programa
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

    program.append("\nsection .text\nglobal _start\n");
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
