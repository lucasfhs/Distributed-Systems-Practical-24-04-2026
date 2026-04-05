#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>
#include <ctime> // para srand
#include <mutex>

using namespace std;

template <size_t N>
class Producer {
public:
    Producer(int num_numbers, array<int, N>& shared_memory, mutex& mtx)
        : num_numbers(num_numbers), shared_memory(shared_memory), mtx(mtx) {
        srand(time(nullptr)); // inicializa rand
    }

    void run() {
        for (int i = 0; i < num_numbers; ++i) {
            int value = generate_number();

            // Bloqueia mutex para acessar a memória compartilhada
            mtx.lock();

            // Procura uma posição livre (-1 = livre)
            for (size_t j = 0; j < N; ++j) {
                if (shared_memory[j] == -1) {
                    shared_memory[j] = value;
                    cout << "Producer colocou " << value << " na posição " << j << endl;
                    break;
                }
            }

            mtx.unlock();

            usleep(100000); // 0,1s de delay para simular produção
        }
    }

private:
    array<int, N>& shared_memory; // referência para o array externo
    int num_numbers;
    mutex& mtx; // para proteger acesso ao array compartilhado

    int generate_number() {
        return rand() % 10000000 + 1; // 1 até 10^7
    }
};