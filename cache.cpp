#include <iostream>
#include <cstdio>
#include "mesi.hpp"

Cache::Cache(int block_size, int line_number) {
    this->block_size = block_size;
    this->line_number = line_number;
    this->count = 1;

    lines = new Linha[line_number];

    // inicializa cada linha vazia
    for(int i = 0; i < line_number; i++) {
        lines[i].block = new int[block_size];

        // inicializa cada bloco das linhas com 0
        for(int j = 0; j < block_size; j++) {
            lines[i].block[j] = 0;
        }

        lines[i].state = INVALID;
        lines[i].time = 0;
        lines[i].addr = -1;
    }
}

Cache::~Cache() {
    for(int i = 0; i < line_number; i++) {
        delete[] lines[i].block;
    }

    delete[] lines;
}


// Verifica se a linha com o endereço dado está na cache
// Retorna -1 caso não esteja e o número da linha caso esteja
int Cache::inCache(int addr) {
    for(int i = 0; i < line_number; i++) {
        if(lines[i].addr == addr)
            return i;
    }

    return -1;
}

// Modifica uma palavra de determinada linha da cache
void Cache::modifyLine(int line, int idx, int value) {
    lines[line].block[idx] = value;
    lines[line].state = MODIFIED;
}

// Insere um bloco na cache
void Cache::insertLine(int line, int *block, int addr, CACHE_STATE state) {
    
    // copia o conteúdo do bloco para a linha
    for(int j = 0; j < block_size; j++) {
        lines[line].block[j] = block[j];
    }

    lines[line].addr = addr;
    lines[line].state = state;

    lines[line].time = count++;
}

// Retorna o índice da próxima linha a ser substituída
int Cache::nextLine() {
    int next = 0;

    // acha a linha que entrou primeiro na cache (menor tempo)
    for(int i = 1; i < line_number; i++) {
        if(lines[i].time <= lines[next].time) {
            next = i;
        }
    }

    return next;
}


/* Getters para a cache */

int Cache::getBlockSize() {
    return block_size;
}

int Cache::getLineNumber() {
    return line_number;
}


/* Getters para as linhas */

CACHE_STATE Cache::getState(int line) {
    return lines[line].state;
}

int Cache::getTime(int line) {
    return lines[line].time;
}

int Cache::getAddress(int line) {
    return lines[line].addr;
}

void Cache::getBlock(int line, int *dest) {
    for(int i = 0; i < block_size; i++) {
        dest[i] = lines[line].block[i];
    }
}

int Cache::getWord(int line, int i) {
    return lines[line].block[i];
}

/* SETTERS */

void Cache::setState(int line, CACHE_STATE state) {
    lines[line].state = state;
}


void print_array(int *array, int size) {
    for(int i = 0; i < size; i++) {
        printf(" %03d", array[i]);
    }
}

/* Função para imprimir a cache */
void Cache::print() {
    int *tmp = new int[block_size];

    std::cout << "Addr\t Block";
    for(int j = 0; j < block_size; j++)
        std::cout << "   ";
    std::cout << "\tTime\tState\n";

    for(int i = 0; i < line_number; i++) {        
        std::cout << getAddress(i) << "\t";

        getBlock(i, tmp);
        print_array(tmp, block_size);
        std::cout << "\t" << getTime(i) << "\t";

        switch(getState(i)) {
            case INVALID:
                std::cout << "Invalid\n";
                break;

            case EXCLUSIVE:
                std::cout << "Exclusive\n";
                break;

            case MODIFIED:
                std::cout << "Modified\n";
                break;
            
            case SHARED:
                std::cout << "Shared\n";
        }
    }

    delete[] tmp;
}

