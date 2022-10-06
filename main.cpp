#include <iostream>
#include "mesi.hpp"

int main(void) {
    int bsize = 5;
    Memory m1(bsize, 12);
    DataBus bus(&m1);
    CacheController c1(bsize, 4, &bus);
    CacheController c2(bsize, 4, &bus);
    int adr, value;
    char op;

    do {
        CacheController *tmp;
        int n;

        bus.printAll();
    
        std::cout << "\nCache: ";
        std::cin >> n;

        if (n == 1) 
            tmp = &c1;
        else
            tmp = &c2;

        std::cout << "Ler ou Escrever (r/w): ";
        std::cin >> op;

        std::cout << "Endereço: ";
        std::cin >> adr;

        switch(op) {
            case 'w':
                std::cout << "Valor: ";
                std::cin >> value;

                tmp->write(adr, value);
                break;

            case 'r':
                value = tmp->read(adr);
                std::cout << "Valor lido: " << value << std::endl;
                break;

            default:
                op = 0;
        }

        std::cout << std::endl;
    } while(op);

    return 0;
}
