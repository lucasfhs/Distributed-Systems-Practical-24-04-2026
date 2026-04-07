#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>

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

    cout << "Pipe criado com sucesso!" << endl;
    
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
    }

    cout << "Execução finalizada!" << endl;
    return 0;
}