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
    Producer(array<int, N>& shared_memory, sem_t& empty, sem_t& full, sem_t& mutex)
        : shared_memory(shared_memory), empty(empty), full(full), mutex(mutex) {}

    void run() {
        int value = generate_number();

        sem_wait(&empty);
        sem_wait(&mutex);

        for (size_t j = 0; j < N; ++j) {
            if (shared_memory[j] == 0) {
                shared_memory[j] = value;
                cout << "Producer colocou " << value << " na posição " << j << endl;
                break;
            }
        }

        sem_post(&mutex);
        sem_post(&full);
    }

private:
    array<int, N>& shared_memory;

    int generate_number() {
        return rand() % 10000000 + 1;
    }
};