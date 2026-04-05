#include <iostream>
#include <unistd.h>

using namespace std;

class Consumer {
public:
    void run(int read_fd) {
        int number;

        while (true) {
            read(read_fd, &number, sizeof(int));

            if (number == 0) break;

            if (is_prime(number)) {
                cout << number << " é primo" << endl;
            } else {
                cout << number << " não é primo" << endl;
            }
        }
    }

private:
    bool is_prime(int number) {
        if (number <= 1) return false;

        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) return false;
        }

        return true;
    }
};