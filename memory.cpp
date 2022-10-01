#include <vector>
#include <ctime>
#include <random>
#include <iostream>

class Memory {
    private:
        int *mem;
        int block_size;
        int block_number;

    public:
        Memory(int block_size, int block_number) {
            int total = block_size * block_number;

            this->block_size = block_size;
            this->block_number = block_size;
        
            mem = new int[total];
            
            // Popula a memória com números aleatórios de 1 a 100
            for(int i = 0; i < total; i++) {
                mem[i] = std::rand() % 100 + 1;
            }
        }

        ~Memory() {
            delete[] mem;
        }

        void readBlock(int addr, int *dest) {
            int start = addr * block_size;
            
            for(int i = 0; i < block_size; i++) {
                dest[i] = mem[start + i];
            }
        }

        void writeBlock(int addr, const int *src) {
            int start = addr * block_size;

            for(int i = 0; i < block_size; i++) {
                mem[start + i] = src[i];
            }
        }

        int getBlockSize() {
            return block_size;
        }

        int getBlockNumber() {
            return block_number;
        }
};