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
#include <sstream>
#include <filesystem>

#define N 1000
#define M 100000

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

    // PRODUTORES
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

    // CONSUMIDORES
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

    for (auto& t : threads) t.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);

    return {duration.count(), buffer_usage};
}

// -----------------------------
// APPEND JSON (corrigido)
// -----------------------------
void append_json_block(const string& block) {
    const string filename = "results.json";

    // Se não existe → cria novo
    if (!filesystem::exists(filename)) {
        ofstream file(filename);
        file << "{\n";
        file << block << "\n";
        file << "}\n";
        file.close();
        return;
    }

    // Se existe → append estruturado
    fstream file(filename, ios::in | ios::out);

    // move para antes do último '}'
    file.seekp(-2, ios::end);

    file << ",\n";
    file << block << "\n";
    file << "}\n";

    file.close();
}

// -----------------------------
// ANALYZE
// -----------------------------
void analyze() {
    vector<pair<int,int>> configs = {
        {1,1},{1,2},{1,4},{1,8},
        {2,1},{4,1},{8,1}
    };

    stringstream block;

    block << "  \"" << N << "\": [\n";

    for (size_t j = 0; j < configs.size(); j++) {
        int Np = configs[j].first;
        int Nc = configs[j].second;

        vector<double> times;
        vector<int> buffer_sample;

        for (int k = 0; k < 10; k++) {
            RunResult r = run_once(Np, Nc);
            times.push_back(r.time);

            if (k == 0) buffer_sample = r.buffer_usage;
        }

        block << "    {\"Np\": " << Np
              << ", \"Nc\": " << Nc
              << ", \"times\": [";

        for (size_t t = 0; t < times.size(); t++) {
            block << times[t];
            if (t < times.size() - 1) block << ",";
        }

        block << "], \"buffer_usage\": [";

        for (size_t b = 0; b < buffer_sample.size(); b++) {
            block << buffer_sample[b];
            if (b < buffer_sample.size() - 1) block << ",";
        }

        block << "]}";

        if (j < configs.size() - 1) block << ",";
        block << "\n";
    }

    block << "  ]";

    append_json_block(block.str());

    cout << "Analyze finished for N=" << N << "!\n";
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

    // modo normal
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
