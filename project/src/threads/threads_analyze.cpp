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
#include <string>

#define N 10       // ⚠️ ainda fixo (depois podemos melhorar)
#define M 100000   // quantidade de números

using namespace std;

// -----------------------------
// Estrutura de retorno
// -----------------------------
struct RunResult {
    double time;
    vector<int> buffer_usage;
};

// -----------------------------
// Executa UMA vez
// -----------------------------
RunResult run_once(int Np, int Nc) {
    array<int, N> shared_memory = {};

    sem_t sem_empty;
    sem_t sem_full;
    sem_t sem_mutex;

    sem_init(&sem_empty, 0, N);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_mutex, 0, 1);

    vector<thread> threads;
    int produced_count = 0;
    int consumed_count = 0;
    mutex count_mutex;

    vector<int> buffer_usage;
    mutex buffer_usage_mutex;

    auto start = chrono::high_resolution_clock::now();

    // -----------------------------
    // PRODUTORES
    // -----------------------------
    for (int i = 0; i < Np; i++) {
        threads.emplace_back([&]() {
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

    // -----------------------------
    // CONSUMIDORES
    // -----------------------------
    for (int i = 0; i < Nc; i++) {
        threads.emplace_back([&]() {
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

    // Join
    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);

    return {duration.count(), buffer_usage};
}

// -----------------------------
// ANALYZE MODE
// -----------------------------
void analyze() {
    vector<int> Ns = {1, 10, 100, 1000};

    vector<pair<int,int>> configs = {
        {1,1},{1,2},{1,4},{1,8},
        {2,1},{4,1},{8,1}
    };

    ofstream file("results.json");

    file << "{\n";

    for (size_t i = 0; i < Ns.size(); i++) {
        int current_N = Ns[i];

        file << "  \"" << current_N << "\": [\n";

        for (size_t j = 0; j < configs.size(); j++) {
            int Np = configs[j].first;
            int Nc = configs[j].second;

            vector<double> times;
            vector<int> buffer_sample;

            // 🔁 roda 10 vezes
            for (int k = 0; k < 10; k++) {
                RunResult result = run_once(Np, Nc);
                times.push_back(result.time);

                // salva buffer só da primeira execução
                if (k == 0) {
                    buffer_sample = result.buffer_usage;
                }
            }

            // -----------------------------
            // JSON write
            // -----------------------------
            file << "    {\"Np\": " << Np
                 << ", \"Nc\": " << Nc
                 << ", \"times\": [";

            for (size_t t = 0; t < times.size(); t++) {
                file << times[t];
                if (t < times.size() - 1) file << ",";
            }

            file << "], \"buffer_usage\": [";

            for (size_t b = 0; b < buffer_sample.size(); b++) {
                file << buffer_sample[b];
                if (b < buffer_sample.size() - 1) file << ",";
            }

            file << "]}";

            if (j < configs.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ]";
        if (i < Ns.size() - 1) file << ",";
        file << "\n";
    }

    file << "}\n";
    file.close();

    cout << "Analyze finished! JSON saved to results.json\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main(int argc, char* argv[]) {
    srand(time(nullptr));

    // modo analyze
    if (argc >= 2 && string(argv[1]) == "analyze") {
        analyze();
        return 0;
    }

    // -----------------------------
    // modo normal (execução única)
    // -----------------------------
    if (argc < 3) {
        cout << "Usage:\n";
        cout << "./bin/thread <Np> <Nc>\n";
        cout << "./bin/thread analyze\n";
        return 1;
    }

    int Np = atoi(argv[1]);
    int Nc = atoi(argv[2]);

    RunResult result = run_once(Np, Nc);

    cout << "Execution finished!\n";
    cout << "Time: " << result.time << " seconds\n";

    return 0;
}