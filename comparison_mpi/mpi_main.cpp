#include <mpi.h>
#include <vector>
#include <random>

long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // int total_rows = 60000;
    int cols = 100000;

    // Each process computes its own portion of the matrix
    int rows_per_process = cols / size;
    // std::vector<std::vector<int>> matrix(rows_per_process, std::vector<int>(cols));
    // std::cout << "Rank " << rank << " has " << rows_per_process << " rows and " << cols << " columns" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 0; i < rows_per_process; ++i) {
        std::cout << "F(" << 5 << ") = " << fibonacci(32) << std::endl;
    }

    // Gather the results
    // if (rank == 0) {
    //     std::vector<std::vector<int>> full_matrix(total_rows, std::vector<int>(cols));
    //     std::copy(matrix.begin(), matrix.end(), full_matrix.begin());
    //     for (int i = 1; i < size; ++i) {
    //         MPI_Recv(&full_matrix[i * rows_per_process][0], rows_per_process * cols, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     }
    //     // full_matrix now contains the complete matrix
    // } else {
    //     MPI_Send(&matrix[0][0], rows_per_process * cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // }

    MPI_Finalize();
    return 0;
}