#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>
#include <ctime>
#include <semaphore.h>

using namespace std;

template <size_t N>
class Consumer {
public:
    Consumer(array<int, N>& shared_memory, sem_t& sem_empty, sem_t& sem_full, sem_t& sem_mutex)
        : shared_memory(shared_memory), sem_empty(sem_empty), sem_full(sem_full), sem_mutex(sem_mutex) {}

    void run() {
        sem_wait(&sem_full);
        sem_wait(&sem_mutex);
        
        for (size_t j = 0; j < N; ++j) {
            if (shared_memory[j] != 0) {
                int value = shared_memory[j];
                shared_memory[j] = 0;
                if(is_prime(value)){
                    cout << "Consumer consumiu um número primo" << value << " da posição " << j << endl;
                }else{
                    cout << "Consumer consumiu um número não primo" << value << " da posição " << j << endl;
                }
                break;
            }
        }

        sem_post(&sem_mutex);
        sem_post(&sem_empty);
    }

private:
    array<int, N>& shared_memory;
    sem_t& sem_empty;
    sem_t& sem_full;
    sem_t& sem_mutex;
    
    bool is_prime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
};