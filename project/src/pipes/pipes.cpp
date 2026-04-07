#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <sys/wait.h>

#include "producer.hpp"
#include "consumer.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Rode o comando: ./bin/pipes <numeros_para_serem_gerados>" << endl;
        return 1;
    }

    int numbers_to_generate = atoi(argv[1]);

    int pipe_fd[2];

    // cria pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }
   
    auto start = chrono::high_resolution_clock::now();
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // =========================
        // FILHO → CONSUMIDOR
        // =========================
        close(pipe_fd[1]); // Fecha o pipe de escrita

        Consumer consumer;
        consumer.run(pipe_fd[0]);

        close(pipe_fd[0]); // Fecha o pipe de leitura 
    } else {
        // =========================
        // PAI → PRODUCER
        // =========================
        close(pipe_fd[0]); // fecha leitura

        srand(time(NULL));

        Producer producer(numbers_to_generate);
        producer.run(pipe_fd[1]);

        close(pipe_fd[1]);
        wait(NULL);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        cout << "Tempo total: " << duration.count() << " ms" << endl;
    }

    if (pid != 0) {
        cout << "Execução finalizada!" << endl;
    }
    return 0;
}