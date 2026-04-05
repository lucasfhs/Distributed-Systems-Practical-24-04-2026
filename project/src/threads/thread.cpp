#include <iostream>
#include <array>
#include "producer.hpp"
#include "consumer.hpp"

# define SHARED_MEMORY_SIZE 10

using namespace std;

int main(int argc, char* argv[]){
    array<long long, SHARED_MEMORY_SIZE> shared_memory = {};

    if (argc < 2) {
        cout << "Rode o comando: ./bin/thread <n_produtores> <n_consumidores>" << endl;
        return 1;
    }
    int qtd_producers = atoi(argv[1]);
    int qtd_consumers = atoi(argv[2]);



    return 0;
}