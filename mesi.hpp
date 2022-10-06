#ifndef MESI_H
#define MESI_H

#include <vector>

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
        Cache(int block_size, int line_number);
        ~Cache();
        int inCache(int addr);
        int nextLine();

        void modifyLine(int line, int idx, int value);
        void insertLine(int line, int *block, int addr, CACHE_STATE state);

        int getBlockSize();
        int getLineNumber();

        CACHE_STATE getState(int line);
        int getTime(int line);
        int getAddress(int line);
        void getBlock(int line, int *dest);
        int getWord(int line, int i);

        void setState(int line, CACHE_STATE state);

        void print();
};

class Memory {
    private:
        int *mem;
        int block_size;
        int block_number;

    public:
        Memory(int block_size, int block_number);
        ~Memory();

        void getBlock(int addr, int *dest);
        void setBlock(int addr, const int *src);

        int getBlockSize();
        int getBlockNumber();

        void print();
};


class CacheController;
class DataBus {
    private:
        std::vector<CacheController*> caches;
        Memory *memory;

    public:
        DataBus(Memory *m);
        
        void addCache(CacheController *c);
        void readMemory(int addr, int *dest);
        void writeMemory(int addr, int *src);

        bool broadcastRead(CacheController *src, int addr);
        void broadcastWrite(CacheController *src, int addr);

        void sendLine(int addr, int *block);

        void printAll();
};

class CacheController {
    private:
        DataBus *bus;
        Cache *cache;
    
    protected:
        void writeMemory(int line);
        void readMemory(int line, int addr, CACHE_STATE state);
        int readLine(int block, CACHE_STATE state=EXCLUSIVE);

    public:
        CacheController(int block_size, int line_number, DataBus *bus);
        ~CacheController();

        void write(int addr, int word);
        int read(int addr);

        bool busRead(int block);
        void busWrite(int block);

        void receiveLine(int addr, int *block);

        void printCache();
};

#endif