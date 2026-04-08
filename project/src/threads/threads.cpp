#include <iostream>
#include <array>
#include <semaphore.h>
#include "producer.hpp"
#include "consumer.hpp"
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <chrono>
#include <fstream>


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
    int produced_count = 0;
    int consumed_count = 0;
    mutex count_mutex;
    vector<int> buffer_usage;
    mutex buffer_usage_mutex;
    buffer_usage.clear();
    
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < Np; i++) {
        threads.emplace_back([&shared_memory, &sem_empty, &sem_full, &sem_mutex, &count_mutex, &produced_count, &buffer_usage, &buffer_usage_mutex]() {
            Producer<N> p(shared_memory, sem_empty, sem_full, sem_mutex, buffer_usage, buffer_usage_mutex);
            while (true) {
                {   
                    lock_guard<mutex> lock(count_mutex);
                    if (produced_count >= M) break;
                    produced_count++;
                }

                p.run();
            }
        });
    }

    for (int i = 0; i < Nc; i++) {
        threads.emplace_back([&shared_memory, &sem_empty, &sem_full, &sem_mutex, &count_mutex, &consumed_count, &buffer_usage, &buffer_usage_mutex]() {
            Consumer<N> c(shared_memory, sem_empty, sem_full, sem_mutex, buffer_usage, buffer_usage_mutex);
            while (true) {
                {
                    lock_guard<mutex> lock(count_mutex);
                    if (consumed_count >= M) break;
                    consumed_count++;
                }

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

    std::ofstream file("results.json");

    file << "{\n";
    file << "  \"N\": " << N << ",\n";
    file << "  \"Np\": " << Np << ",\n";
    file << "  \"Nc\": " << Nc << ",\n";
    file << "  \"tempo\": " << duration.count() << ",\n";

    file << "  \"buffer_usage\": [";

    for (size_t i = 0; i < buffer_usage.size(); i++) {
        file << buffer_usage[i];
        if (i != buffer_usage.size() - 1) file << ",";
    }

    file << "]\n";
    file << "}";

    file.close();

    cout << "Execução finalizada!" << endl;
    cout << "Total consumido: " << consumed_count << endl;
    cout << "Tempo total: " << duration.count() << " segundos" << endl;

    return 0;
}