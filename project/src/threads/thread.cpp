#include <iostream>
#include <array>
#include <semaphore.h>
#include "producer.hpp"
#include "consumer.hpp"
#include <thread>
#include <vector>
#include <ctime>
// SHARED_MEMORY_SIZE
# define N 10

using namespace std;

int main(int argc, char* argv[]){
    srand(time(nullptr));
    
    array<int, N> shared_memory = {};

    sem_t empty;
    sem_t full;
    sem_t mutex;

    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    if (argc < 3) {
        cout << "Rode o comando: ./bin/thread <n_produtores> <n_consumidores>" << endl;
        return 1;
    }
    int qtd_producers = atoi(argv[1]);
    int qtd_consumers = atoi(argv[2]);

    vector<thread> threads;

    for (int i = 0; i < qtd_producers; i++) {
        threads.emplace_back([&shared_memory]() {
            Producer<N> p(shared_memory, empty, full, mutex);
            while (true) p.run();
        });
    }

    for (int i = 0; i < qtd_consumers; i++) {
        threads.emplace_back([&shared_memory]() {
            Consumer<N> c(shared_memory, empty, full, mutex);
            while (true) c.run();
        });
    }

    for (auto& t : threads) {
        t.join();
    }



    return 0;
}