#include "mesi.hpp"
#include <iostream>

/*
   CacheController - Controlador de Cache

   Implementa a lógica do protocolo MESI e realiza leitura
   e escrita em linhas da cache e em blocos correspondentes na memória 
*/

/* Escreve o conteúdo da linha 'int line' da cache
   na memória principal através do barramento */
void CacheController::writeMemory(int line) {
    int *tmp = new int[cache->getBlockSize()];

    cache->getBlock(line, tmp);
    bus->writeMemory(cache->getAddress(line), tmp);

    delete[] tmp;
}

/* Lê o bloco de número 'int addr' da memória principal
   através do barramento e o insere na cache */
void CacheController::readMemory(int line, int addr, CACHE_STATE state) {
    int *tmp = new int[cache->getBlockSize()];

    bus->readMemory(addr, tmp);
    cache->insertLine(line, tmp, addr, state);

    delete[] tmp;
}

/* Retorna a linha da cache em que está armazenado o bloco 
   de número 'int block'. Caso ele não esteja na cache, ele é lido
   e armazenado na próxima linha a ser substituída */
int CacheController::readLine(int block, CACHE_STATE state) {
    int next = cache->nextLine();
    int line = cache->inCache(block);

    if(line == -1) {
        std::cout << "MISS\n";

        if(cache->getState(next) == MODIFIED) {
            writeMemory(next); // writeback
        }

        readMemory(next, block, state);

        line = next;

    } else {
        std::cout << "HIT\n";
    }

    return line;
}

/* Inicializa o controlador de cache, criando uma cache com  
   'int line_number' linhas e blocos de tamanho 'int block_size'. 
   Associa a cache ao barramento 'DataBus *bus' */
CacheController::CacheController(int block_size, int line_number, DataBus *bus) {
    this->cache = new Cache(block_size, line_number);
    this->bus = bus;
    bus->addCache(this);
}

CacheController::~CacheController() {
    delete cache;
}

/* Avisa as outras caches no barramento que irá ocorrer uma escrita
   no bloco do endereço 'int addr' 

   Modifica a linha associada ao endereço de memória 'int addr' 
   para que o endereço correspondente contenha a palavra 'int word', 
   lendo o bloco da memória principal caso não esteja na cache. */
void CacheController::write(int addr, int word) {
    int i, b, line, bsize = cache->getBlockSize();
    
    i = addr % bsize;
    b = addr / bsize;

    bus->broadcastWrite(this, b);

    std::cout << "\nWRITE ";

    line = readLine(b, EXCLUSIVE);
    cache->modifyLine(line, i, word);
}

/* Avisa as outras caches no barramento que irá ocorrer uma
   leitura no bloco do endereço 'int addr'

   Retorna o valor do endereço de memória 'int addr' armazenado
   na linha correspondente, lendo o bloco da memória principal
   caso não esteja na cache. */
int CacheController::read(int addr) {
    int bsize = cache->getBlockSize();
    CACHE_STATE state = EXCLUSIVE;
    int i, b, line;
    
    i = addr % bsize;
    b = addr / bsize;

    if(bus->broadcastRead(this, b)) {
        state = SHARED;
    }

    std::cout << "\nREAD ";

    line = readLine(b, state);

    return cache->getWord(line, i);            
}

/* Reage a uma leitura do bloco 'int block' que irá ocorrer em
   outra cache conectada ao barramento. Caso a mesma linha esteja 
   na cache, muda seu estado para compartilhada. Caso a linha tenha
   sido modificada, a escreve de volta na memória para que seja lida
   pela outra cache. */   
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

/* Reage a uma escrita no bloco 'int block' que irá ocorrer em
   outra cache conectada ao barramento. Caso a mesma linha esteja 
   na cache, muda seu estado para inválida. Caso a linha tenha
   sido modificada, a escreve de volta na memória para que seja lida
   e modificada pela outra cache. */ 
void CacheController::busWrite(int block) {
    int line = cache->inCache(block);

    if(line != -1) {
        if(cache->getState(line) == MODIFIED) {
            writeMemory(line);
        }
        cache->setState(line, INVALID);
        cache->setTime(line, 0);
    }
}

/* Imprime o conteúdo da cache na tela */
void CacheController::printCache() {
    cache->print();
}
