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
        struct Linha {
            CACHE_STATE mesi;
            int time; 
            int *block;
            int addr;
        } *lines;

        int block_size;
        int line_number;
        int count;

    public:
        Cache(int block_size, int line_number) {
            this->block_size = block_size;
            this->line_number = line_number;
            this->count = 1;

            lines = new Linha[line_number];

            for(int i = 0; i < line_number; i++) {
                lines[i].block = new int[block_size];

                for(int j = 0; j < block_size; j++) {
                    lines[i].block[j] = 0;
                }

                lines[i].mesi = INVALID;
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

        int getBlockSize() {
            return block_size;
        }

        int getLineNumber() {
            return line_number;
        }

        CACHE_STATE getState(int line) {
            return lines[line].mesi;
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

        int inCache(int addr) {
            for(int i = 0; i < line_number; i++) {
                if(lines[i].addr == addr)
                    return i;
            }

            return -1;
        }

        void modifyLine(int line, int idx, int value) {
            lines[line].block[idx] = value;
            lines[line].mesi = MODIFIED;
        }

        // Retorna o índice da próxima linha a ser substituída
        int nextLine() {
            int next = 0;

            for(int i = 1; i < line_number; i++) {
                if(lines[i].time <= lines[next].time) {
                    next = i;
                }
            }

            return next;
        }

        // Insere um bloco na cache
        void insertLine(int *block, int addr, CACHE_STATE state) {
            int i = nextLine();

            for(int j = 0; j < block_size; j++) {
                lines[i].block[j] = block[j];
            }

            lines[i].addr = addr;
            lines[i].mesi = state;

            lines[i].time = count;
            count++;
        }
};
