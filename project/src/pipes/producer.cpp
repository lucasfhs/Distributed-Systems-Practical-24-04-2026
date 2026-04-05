class Producer {
    public:
        Producer(int num_numbers) {
            this->num_numbers = num_numbers;
        }

        void produce() {
            // Gera os números e imprime na saída padrão
            for (int i = 0; i < num_numbers; i++) {
                int next_number = generate_number(previous_number);
                cout << next_number << endl;
                previous_number = next_number;
            }
        }

    private:
        int num_numbers;
        int previous_number = 0;

        int generate_number(int previous_number) {
            // Formula para gerar o próximo número sugerida no enunciado
            // Ni= Ni−1 +∆, N0 =1, ∆∈[1,100].
            int delta = rand() % 100 + 1;
            return previous_number + delta;
        }
}