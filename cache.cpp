#include <iostream>
#include <cstdio>

enum CACHE_STATE {
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

class Cache {
    private:
        int block_size; // tamanho do bloco
        int line_number; // número de linhas da cache
        int count; // contador para determinar quando uma linha entrou na cache

        struct Linha {
            CACHE_STATE state; // estado MESI da linha da cache
            int time; // valor do contador quando a linha entrou na cache
            int *block;
            int addr; // endereço (número do bloco na memória)
        } *lines;

    public:
        Cache(int block_size, int line_number) {
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

        ~Cache() {
            for(int i = 0; i < line_number; i++) {
                delete[] lines[i].block;
            }

            delete[] lines;
        }


        // Verifica se a linha com o endereço dado está na cache
        // Retorna -1 caso não esteja e o número da linha caso esteja
        int inCache(int addr) {
            for(int i = 0; i < line_number; i++) {
                if(lines[i].addr == addr)
                    return i;
            }

            return -1;
        }

        // Modifica uma palavra de determinada linha da cache
        void modifyLine(int line, int idx, int value) {
            lines[line].block[idx] = value;
            lines[line].state = MODIFIED;
        }

        // Insere um bloco na cache
        void insertLine(int line, int *block, int addr, CACHE_STATE state) {
            
            // copia o conteúdo do bloco para a linha
            for(int j = 0; j < block_size; j++) {
                lines[line].block[j] = block[j];
            }

            lines[line].addr = addr;
            lines[line].state = state;

            lines[line].time = count++;
        }

        // Retorna o índice da próxima linha a ser substituída
        int nextLine() {
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

        int getBlockSize() {
            return block_size;
        }

        int getLineNumber() {
            return line_number;
        }


        /* Getters para as linhas */

        CACHE_STATE getState(int line) {
            return lines[line].state;
        }

        int getTime(int line) {
            return lines[line].time;
        }

        int getAddress(int line) {
            return lines[line].addr;
        }

        void getBlock(int line, int *dest) {
            for(int i = 0; i < block_size; i++) {
                dest[i] = lines[line].block[i];
            }
        }
};
