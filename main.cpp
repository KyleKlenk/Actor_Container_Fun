#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <random>
std::vector<std::vector<int>> optimized_matrix_multi(const std::vector<std::vector<int>>& a, 
                                          const std::vector<std::vector<int>>& b) {
    int n = a.size();
    std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

        }
    }
    return c;
}

std::vector<std::vector<int>> parallel_matrix_mult(const std::vector<std::vector<int>>& a, 
                                          const std::vector<std::vector<int>>& b) {
    int n = a.size();
    std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

std::vector<std::vector<int>> matrix_mult(const std::vector<std::vector<int>>& a, 
                                          const std::vector<std::vector<int>>& b) {
    int n = a.size();
    std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}


std::vector<std::vector<int>> generate_random_matrix(int n) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 100;  // Generate random values between 0 and 99
        }
    }
    return matrix;
}

std::vector<int> generate_random_vector(int n) {
    std::vector<int> a(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 0; i < n; i++) {
        a[i] = dis(gen);  // Generate random values between 0 and 99
    }
    return a;
}

std::vector<int> vector_add_parallel(const std::vector<int>& a, const std::vector<int>& b) {
    int n = a.size();
    std::vector<int> c(n);
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Number of threads = " << omp_get_num_threads() << std::endl;
        }
        #pragma omp for
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
        }
    }
    return c;
}

std::vector<int> vector_add(const std::vector<int>& a, const std::vector<int>& b) {
    int n = a.size();
    std::vector<int> c(n);
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
    return c;
}

int main() {
    // srand(time(0));  // Seed the random number generator
    int n = 4000;  // Size of the matrices
    auto a = generate_random_matrix(n);
    auto b = generate_random_matrix(n);
    // auto c = vector_add_parallel(a, b);÷
    // std::vector<int> c = vector_add(a, b)

    // std::vector<std::vector<int>> c = parallel_matrix_mult(a, b);
    auto c = matrix_mult(a, b);
    // for (const auto& row : c) {
    //     for (const auto& elem : row) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}