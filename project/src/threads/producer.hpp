#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>
#include <ctime>

using namespace std;

template <size_t N>
class Producer {
public:
    Producer(array<int, N>& shared_memory)
        : shared_memory(shared_memory) {
        srand(time(nullptr));
    }

    void run() {
        int value = generate_number();
        for (size_t j = 0; j < N; ++j) {
            if (shared_memory[j] == 0) {
                shared_memory[j] = value;
                cout << "Producer colocou " << value << " na posição " << j << endl;
                break;
            }
        }
        // Delay para simular produção
        // usleep(100000);
    }

private:
    array<int, N>& shared_memory;

    int generate_number() {
        return rand() % 10000000 + 1;
    }
};