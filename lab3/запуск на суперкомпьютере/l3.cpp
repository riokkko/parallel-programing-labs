#include <iostream>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <mpi.h>

void begining();
void set_matrix(const std::string& f, std::vector<std::vector<double>>& matrix);
void write_matrix(const std::string& f, const std::vector<std::vector<double>>& matrix);
bool can_multyply(const std::vector<std::vector<double>>& matrix1,
	const std::vector<std::vector<double>>& matrix2);

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) begining();

    if (argc != 4) {
        if (rank == 0) {
            std::cout << "Использование: " << argv[0] << " A.txt B.txt C.txt\n";
        }
        MPI_Finalize();
        return 1;
    }

    try {
        int N = 0;
        std::vector<std::vector<double> > A, B;

        if (rank == 0) {
            set_matrix(argv[1], A);
            set_matrix(argv[2], B);
            can_multyply(A, B);
	    N = A.size();
        }

        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (N % size != 0) {
            if (rank == 0) {
                std::cerr << "Ошибка: размер матрицы (" << N << ") не делится на число процессов (" << size << ")\n";
            }
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int rows_per_proc = N / size;

        std::vector<double> flat_B(N * N);
        if (rank == 0) {
            for (int t = 0; t < N; t++)
                for (int i = 0; i < N; i++)
                    flat_B[t * N + i] = B[t][i];
        }

        auto t1 = std::chrono::high_resolution_clock::now();

        MPI_Bcast(flat_B.data(), N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        std::vector<double> flat_A;
        if (rank == 0) {
            flat_A.resize(N * N);
            for (int t = 0; t < N; t++)
                for (int i = 0; i < N; i++)
                    flat_A[t * N + i] = A[t][i];
        }

        std::vector<double> local_A(rows_per_proc * N);
        MPI_Scatter(flat_A.data(), rows_per_proc * N, MPI_DOUBLE,
                    local_A.data(), rows_per_proc * N, MPI_DOUBLE, 0,
                    MPI_COMM_WORLD);

        std::vector<double> local_C(rows_per_proc * N, 0.0);

        for (int t = 0; t < rows_per_proc; t++) {
            for (int i = 0; i < N; i++) {
                double a_ti = local_A[t * N + i];
                for (int h = 0; h < N; h++) {
                    local_C[t * N + h] += a_ti * flat_B[i * N + h];
                }
            }
        }

        std::vector<double> flat_C;
        if (rank == 0) flat_C.resize(N * N);

        MPI_Gather(local_C.data(), rows_per_proc * N, MPI_DOUBLE,
                   flat_C.data(), rows_per_proc * N, MPI_DOUBLE, 0,
                   MPI_COMM_WORLD);

        auto t2 = std::chrono::high_resolution_clock::now();

        if (rank == 0) {
            std::vector<std::vector<double> > C(N, std::vector<double>(N));
            for (int t = 0; t < N; t++) {
                for (int i = 0; i < N; i++) {
                    C[t][i] = flat_C[t * N + i];
                }
            }

            write_matrix(argv[3], C);

            double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
            long long ops = 2LL * N * N * N;

            std::cout << "Кол-во процессов: " << size << "\n";
            std::cout << "N = " << N << "\n";
            std::cout << "Время = " << ms << " мс\n";
            std::cout << "Объём задачи = " << ops << " операций\n";
            std::cout << "Время на операцию = " << (ms * 1e6 / ops) << " нс\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка в процессе " << rank << ": " << e.what() << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Finalize();
    return 0;
}