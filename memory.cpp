#include <random>
#include <iostream>
#include "mesi.hpp"

Memory::Memory(int block_size, int block_number) {
    int total = block_size * block_number;

    this->block_size = block_size;
    this->block_number = block_number;

    mem = new int[total];
    
    // Popula a memória com números aleatórios de 1 a 100
    for(int i = 0; i < total; i++) {
        mem[i] = std::rand() % 100 + 1;
    }
}

Memory::~Memory() {
    delete[] mem;
}

void Memory::getBlock(int addr, int *dest) {
    int start = addr * block_size;
    
    for(int i = 0; i < block_size; i++) {
        dest[i] = mem[start + i];
    }
}

void Memory::setBlock(int addr, const int *src) {
    int start = addr * block_size;

    for(int i = 0; i < block_size; i++) {
        mem[start + i] = src[i];
    }
}

int Memory::getBlockSize() {
    return block_size;
}

int Memory::getBlockNumber() {
    return block_number;
}

/* Função pra imprimir a memória */
void Memory::print() {
    std::string str;

    int k = 80 / (block_size * 4);
    int *tmp = new int[block_size];
    char num[4];

    for (int block = 0; block < block_number; block++) {
        getBlock(block, tmp);

        for(int i = 0; i < block_size; i++) {
            sprintf(num, "%03d", tmp[i]);
            str += std::string(num);

            if(i + 1 < block_size)
                str += " ";
        }

        if ((block + 1) % k == 0 || (block + 1) >= block_number)
            str += "\n";
        else
            str += "\t\t";
    }

    delete [] tmp;

    std::cout << str;
}
