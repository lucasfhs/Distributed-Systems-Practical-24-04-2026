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

    sem_t sem_empty;
    sem_t sem_full;
    sem_t sem_mutex;

    sem_init(&sem_empty, 0, N);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_mutex, 0, 1);

    if (argc < 3) {
        cout << "Rode o comando: ./bin/thread <n_produtores> <n_consumidores>" << endl;
        return 1;
    }

    int Np = atoi(argv[1]); // Número de produtores
    int Nc = atoi(argv[2]); // Número de consumidores

    vector<thread> threads;
    atomic<int> produced_count(0);
    atomic<int> consumed_count(0);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < Np; i++) {
        threads.emplace_back([&shared_memory, &sem_empty, &sem_full, &sem_mutex, &consumed_count]() {
            Producer<N> p(shared_memory, sem_empty, sem_full, sem_mutex);
            int prev = produced_count.fetch_add(1);
            while (prev >= M) break;
            p.run();
        });
    }

    for (int i = 0; i < Nc; i++) {
        threads.emplace_back([&shared_memory, &sem_empty, &sem_full, &sem_mutex, &consumed_count]() {
            Consumer<N> c(shared_memory, sem_empty, sem_full, sem_mutex);

            while (true) {
                int prev = consumed_count.fetch_add(1);
                if (prev >= M) break;
                c.run();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);

    cout << "Execução finalizada!" << endl;
    cout << "Total consumido: " << consumed_count << endl;
    cout << "Tempo total: " << duration.count() << " segundos" << endl;

    return 0;
}