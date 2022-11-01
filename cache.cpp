#include <iostream>
#include <cstdio>
#include "mesi.hpp"

/*  Inicializa a memória cache com 'line_number' linhas
    e blocos de tamanho 'block_size' (devem ser o mesmo tamanho
    dos blocos da memória). 
    
    Inicializa o contador para registrar o tempo em que cada nova
    linha entra na cache (para o algoritmo de substituição FIFO)
    
    Aloca espaço para os blocos de cada linha */
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

/* Desaloca a memória de cada linha */
Cache::~Cache() {
    for(int i = 0; i < line_number; i++) {
        delete[] lines[i].block;
    }

    delete[] lines;
}


/*  Verifica se a linha de bloco número 'block_num' está na cache
    Retorna -1 caso não esteja e o número da linha caso esteja */
int Cache::inCache(int block_num) {
    for(int i = 0; i < line_number; i++) {
        if(lines[i].addr == block_num && lines[i].state != INVALID)
            return i;
    }

    return -1;
}

/*  Modifica a palavra de indice 'idx' da linha 'line' da cache
    com o novo valor 'value'  */
void Cache::modifyLine(int line, int idx, int value) {
    lines[line].block[idx] = value;
    lines[line].state = MODIFIED;
}

/*  Insere uma nova linha na cache, na linha 'line', copiando o conteúdo armazenado
    em 'block', armazenando na linha o número do bloco 'block_num' e o estado 'state' */
void Cache::insertLine(int line, int *block, int block_num, CACHE_STATE state) {
    
    // copia o conteúdo do bloco para a linha
    for(int j = 0; j < block_size; j++) {
        lines[line].block[j] = block[j];
    }

    lines[line].addr = block_num;
    lines[line].state = state;

    lines[line].time = count++;
}

/*  Retorna o índice da próxima linha a ser substituída, 
    de acordo com o algoritmo de substituição FIFO. A linha com menor
    tempo de entrada (atributo 'time') é retornada. No caso da cache
    não estar cheia, retorna a última linha inválida com tempo 0 */
int Cache::nextLine() {
    int next = line_number - 1;

    // acha a linha que entrou primeiro na cache (menor tempo)
    for(int i = line_number - 2; i >= 0; i--) {
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

/* Retorna o valor de índice 'i' da linha 'line' */
int Cache::getWord(int line, int i) {
    return lines[line].block[i];
}

/* Setters para a linha */

void Cache::setState(int line, CACHE_STATE state) {
    lines[line].state = state;
}

void Cache::setTime(int line, int time) {
    lines[line].time = time;
}

/* Função auxiliar para imprimir os blocos das linhas da cache */
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

