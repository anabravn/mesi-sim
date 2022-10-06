#include "mesi.hpp"
#include <iostream>

void CacheController::writeMemory(int line) {
    int *tmp = new int[cache->getBlockSize()];

    cache->getBlock(line, tmp);
    bus->writeMemory(cache->getAddress(line), tmp);

    delete[] tmp;
}

void CacheController::readMemory(int line, int addr, CACHE_STATE state) {
    int *tmp = new int[cache->getBlockSize()];

    bus->readMemory(addr, tmp);
    cache->insertLine(line, tmp, addr, state);

    delete[] tmp;
}

int CacheController::readLine(int block, CACHE_STATE state) {
    int next = cache->nextLine();
    int line = cache->inCache(block);

    if(line == -1) {
        if(cache->getState(next) == MODIFIED) {
            writeMemory(next);
        }

        readMemory(next, block, state);

        line = next;
    }

    return line;
}

CacheController::CacheController(int block_size, int line_number, DataBus *bus) {
    this->cache = new Cache(block_size, line_number);
    this->bus = bus;
    bus->addCache(this);
}

CacheController::~CacheController() {
    delete cache;
}

void CacheController::write(int addr, int word) {
    int i, b, line, bsize = cache->getBlockSize();
    
    i = addr % bsize;
    b = addr / bsize;

    bus->broadcastWrite(this, b);

    line = readLine(b);
    cache->modifyLine(line, i, word);
}

int CacheController::read(int addr) {
    int bsize = cache->getBlockSize();
    CACHE_STATE state = EXCLUSIVE;
    int i, b, line;
    
    i = addr % bsize;
    b = addr / bsize;

    if(bus->broadcastRead(this, b)) {
        state = SHARED;
    }

    line = readLine(b, state);

    return cache->getWord(line, i);            
}

bool CacheController::busRead(int block) {
    int line = cache->inCache(block);
    bool shared = false;

    if(line != -1) {
        if(cache->getState(line) == MODIFIED) {
            writeMemory(line);
        }

        cache->setState(line, SHARED);
        shared = true;
    } 

    return shared;
}

void CacheController::busWrite(int block) {
    int line = cache->inCache(block);

    if(line != -1) {
        if(cache->getState(line) == MODIFIED) {
            writeMemory(line);
        }
        cache->setState(line, INVALID);
    }
}

void CacheController::receiveLine(int addr, int *block) {
    int line = cache->inCache(addr);

    if (line != -1) {
        cache->insertLine(line, block, addr, SHARED);
    }
}

void CacheController::printCache() {
    cache->print();
}
