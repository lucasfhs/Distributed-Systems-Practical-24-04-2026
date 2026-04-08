#pragma once
#include <array>

template <size_t N>
int get_buffer_size(const std::array<int, N>& shared_memory) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (shared_memory[i] != 0) count++;
    }
    return count;
}