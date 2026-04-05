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
        cout << "Usage: ./pipes <numbers_to_generate>" << endl;
        return 1;
    }

    int numbers_to_generate = atoi(argv[1]);

    int pipe_fd[2];

    // cria pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // =========================
        // FILHO → CONSUMER
        // =========================
        close(pipe_fd[1]); // fecha escrita

        Consumer consumer;
        consumer.run(pipe_fd[0]);

        close(pipe_fd[0]);
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

    return 0;
}