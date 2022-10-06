#include <vector>
#include <iostream>
#include "mesi.hpp"

DataBus::DataBus(Memory *m) {
    memory = m;
};

void DataBus::addCache(CacheController *c) {
    caches.insert(caches.end(), c);
}

void DataBus::readMemory(int addr, int *dest) {
    memory->getBlock(addr, dest);
}

void DataBus::writeMemory(int addr, int *src) {
    memory->setBlock(addr, src);
}

bool DataBus::broadcastRead(CacheController *src, int addr) {
    bool shared = false;

    for (CacheController *c : caches) {
        if(c != src) {
            if(c->busRead(addr))
                shared = true;
        }
    }

    return shared;
}

void DataBus::broadcastWrite(CacheController *src, int addr) {
    for(CacheController *c : caches) {
        if(c != src) {
            c->busWrite(addr);
        }
    }
}

void DataBus::printAll() {
    int i = 1;

    for(CacheController *c : caches) {
        std::cout << "Memória Cache " << i << std::endl;
        c->printCache();
        std::cout << std::endl;

        i++;
    }

    std::cout << "Memória Principal\n";
    memory->print();
}