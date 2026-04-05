class Consumer {
    public:
        Consumer() {
            
        }

        void consume(int number) {
            if (number == 0) {
                return;
            }
            if (is_prime(number)) {
                cout << number << " é primo" << endl;
            } else {
                cout << number << " não é primo" << endl;
            }
        }

    private:
        int is_prime(int number) {
            if (number <= 1) {
                return false;
            }
            for (int i = 2; i < number; i++) {
                if (number % i == 0) {
                    return false;
                }
            }
            return true;    
        }
}