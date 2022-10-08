#include <iostream>
#include <vector>
#include "mesi.hpp"

int main(void) {
    int bsize = 4;
    char op;

    Memory mem(bsize, 10);
    DataBus bus(&mem);

    CacheController cache[] = {CacheController(bsize, 4, &bus), CacheController(bsize, 4, &bus)};

    do {
        int n, adr, value;

        bus.printAll();
    
        std::cout << "\nCache: ";
        std::cin >> n;

        std::cout << "Ler ou Escrever (r/w): ";
        std::cin >> op;

        std::cout << "Endereço: ";
        std::cin >> adr;

        switch(op) {
            case 'w':
                std::cout << "Valor: ";
                std::cin >> value;

                cache[n-1].write(adr, value);
                break;

            case 'r':
                value = cache[n-1].read(adr);
                std::cout << "Valor lido: " << value << std::endl;
                break;

            default:
                op = 0;
        }

        std::cout << std::endl;
    } while(op);

    return 0;
}
