
#include "cache.cpp"
#include "memory.cpp"

/* esse arquivo é só pra testar a cache 
   e a memória, o resto vai ser implementado depois
*/

void print_array(int *array, int size) {
    for(int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
}

void print_mem(Memory *m) {
    int *tmp = new int[m->getBlockSize()];

    std::cout << "Memória Principal\n";
    for(int i = 0; i < m->getBlockNumber(); i++) {
        m->readBlock(i, tmp);

        for(int j = 0; j < m->getBlockSize(); j++) {
            std::cout << tmp[j] << " ";
        }
        std::cout << "\t";
    }

    delete[] tmp;
}

void print_cache(Cache *c) {
    int bsize = c->getBlockSize();
    int lines = c->getLineNumber();
    int *tmp = new int[bsize];

    std::cout << "Memória Cache\n";

    std::cout << "Addr\tBlock";
    for(int j = 0; j < bsize; j++)
        std::cout << "  ";
    std::cout << "\tTime\tState\n";

    for(int i = 0; i < lines; i++) {        
        std::cout << c->getAddress(i) << "\t";

        c->getBlock(i, tmp);
        print_array(tmp, bsize);
        std::cout << "\t" << c->getTime(i) << "\t" << c->getState(i) << std::endl;
    }

    delete[] tmp;
}

int main(void) {
    int bsize = 4;
    Cache c1(bsize,2);
    Memory m1(bsize, 10);
    int adr;
    int block[4];
    char op;

    do {
        print_cache(&c1);
        std::cout << std::endl;
        print_mem(&m1);
        std::cout << std::endl;

        std::cout << "\nLer ou Escrever (r/w): ";
        std::cin >> op;

        if(op == 'r' || op == 'w') {
            std::cout << "Endereço: ";
            std::cin >> adr;

            int b = adr / bsize;
            int i = adr % bsize;

            int next, line;

            switch(op) {
                case 'r':
                    line = c1.inCache(b);
                    if(line == -1) {
                        m1.readBlock(b, block);
                        c1.insertLine(block, b, EXCLUSIVE);
                    }
                    break;

                case 'w':
                    int value;
                    
                    line = c1.inCache(b);

                    std::cout << "Novo valor: ";
                    std::cin >> value;

                    next = c1.nextLine();

                    // write back
                    if (c1.getState(next) == MODIFIED) {
                        c1.getBlock(next, block);
                        m1.writeBlock(c1.getAddress(next), block);
                    }

                    if (line == -1) {
                        m1.readBlock(b, block);
                        c1.insertLine(block, b, EXCLUSIVE);
                    }

                    line = c1.inCache(b);
                    if(line != -1) {
                        c1.modifyLine(line, i, value);
                    }

                    break;
            }
        } else op = 0;

        std::cout << std::endl;
    } while(op);

    return 0;
}