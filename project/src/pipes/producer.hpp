#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

class Producer {
public:
    Producer(int num_numbers) {
        this->num_numbers = num_numbers;
    }

    void run(int write_fd) {
        int previous_number = 1;

        for (int i = 0; i < num_numbers; i++) {
            int next_number = generate_number(previous_number);
            cout << "Produtor: " << next_number << endl;
            write(write_fd, &next_number, sizeof(int));

            previous_number = next_number;
        }

        int stop = 0;
        write(write_fd, &stop, sizeof(int));
    }

private:
    int num_numbers;

    int generate_number(int previous_number) {
        int delta = rand() % 100 + 1;
        return previous_number + delta;
    }
};