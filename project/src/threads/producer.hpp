#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>
#include <ctime>
#include <semaphore.h>

using namespace std;

template <size_t N>
class Producer {
public:
    Producer(array<int, N>& shared_memory, sem_t& sem_empty, sem_t& sem_full, sem_t& sem_mutex)
        : shared_memory(shared_memory), sem_empty(sem_empty), sem_full(sem_full), sem_mutex(sem_mutex) {}

    void run() {
        int value = generate_number();

        sem_wait(&sem_empty);
        sem_wait(&sem_mutex);

        for (size_t j = 0; j < N; ++j) {
            if (shared_memory[j] == 0) {
                shared_memory[j] = value;
                cout << "Producer colocou " << value << " na posição " << j << endl;
                break;
            }
        }

        sem_post(&sem_mutex);
        sem_post(&sem_full);
    }

private:
    array<int, N>& shared_memory;
    sem_t& sem_empty;
    sem_t& sem_full;
    sem_t& sem_mutex;

    int generate_number() {
        return rand() % 10000000 + 1;
    }
};