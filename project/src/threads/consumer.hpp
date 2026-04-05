#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>
#include <ctime>

using namespace std;

template <size_t N>
class Consumer {
public:
    Consumer(array<int, N>& shared_memory)
        : shared_memory(shared_memory) {
        srand(time(nullptr));
    }

    void run() {
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
        // Delay para simular produção
        // usleep(100000);
    }

private:
    array<int, N>& shared_memory;
    
    bool is_prime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
};