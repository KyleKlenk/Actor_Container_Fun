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

// int main() {
// #include <iostream>

long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int main() {
    int n = 49; // This will take a long time to compute
    for (int i = 0; i < 50000; ++i)
        std::cout << "F(" << 5 << ") = " << fibonacci(30) << std::endl;
    return 0;
// }
}