#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(0) + rank);  // Seed the random number generator

    int n = 2000;  // Size of the matrices
    std::vector<std::vector<int>> a(n, std::vector<int>(n));
    std::vector<std::vector<int>> b(n, std::vector<int>(n));
    std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));

    // Initialize matrices a and b with random values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 100;
            b[i][j] = rand() % 100;
        }
    }

    // Divide the rows of a among the processes
    int rows_per_proc = n / size;
    std::vector<std::vector<int>> local_a(rows_per_proc, std::vector<int>(n));

    MPI_Scatter(a.data(), rows_per_proc * n, MPI_INT, local_a.data(), rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);

    // All processes do the multiplication on their local data
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] += local_a[i][k] * b[k][j];
            }
        }
    }

    // Gather the results back to the root process
    MPI_Gather(c.data(), rows_per_proc * n, MPI_INT, a.data(), rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}