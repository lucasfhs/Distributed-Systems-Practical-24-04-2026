#include <iostream>
#include <array>
#include <semaphore.h>
#include "producer.hpp"
#include "consumer.hpp"
#include <thread>
#include <vector>
#include <ctime>
#include <atomic>
#include <chrono>

#define N 10      // tamanho da memoria compartilhada
#define M 100000  // quantidade de números a processar

using namespace std;

int main(int argc, char* argv[]) {
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

    int Np = atoi(argv[1]); // Número de produtores
    int Nc = atoi(argv[2]); // Número de consumidores

    vector<thread> threads;

    atomic<int> consumed_count(0);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < Np; i++) {
        threads.emplace_back([&shared_memory, &empty, &full, &mutex, &consumed_count]() {
            Producer<N> p(shared_memory, empty, full, mutex);

            while (consumed_count < M) {
                p.run();
            }
        });
    }

    for (int i = 0; i < Nc; i++) {
        threads.emplace_back([&shared_memory, &empty, &full, &mutex, &consumed_count]() {
            Consumer<N> c(shared_memory, empty, full, mutex);

            while (true) {
                if (consumed_count >= M) break;

                c.run();
                consumed_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    cout << "Execução finalizada!" << endl;
    cout << "Total consumido: " << consumed_count << endl;
    cout << "Tempo total: " << duration.count() << " segundos" << endl;

    return 0;
}