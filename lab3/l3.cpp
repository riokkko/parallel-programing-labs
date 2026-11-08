import std;
import maker;
import <mpi.h>;

int main (int argc, char** argv) { 
	
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if (rank == 0) begining();
	
	if (argc != 4) {
		of (rank == 0) {
			std::println("Использование: {} A.txt B.txt C.txt", argv[0]);
		}
		return 1;
    }
	try {
		int N = 0;
		std::vector<std::vector<double>> A, B;
		
		if (rank == 0) {
			set_matrix(argv[1], A);
			set_matrix(argv[2], B);
			can_multyply(A, B);
			N = A.size();
		}
		
		MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		if (N % size != 0) {
			if (rank == 0) {
				std::cerr << std::format("Размер матрицы: {} ", N)
				<< "Должен делиться на число процессов " << size << "." 
				<< std::endl;
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
			return 1;
		}
		
		int rows_per_proc = N / size;
		
		std::vector<double> flat_B(N * N);
		if (rank == 0) {
			for (int t = 0; t < N; t++)
				for (int i = 0; i < N; i++)
					flat_B[t * N + i] = B[t][i];
		}

		auto t1 = std::chrono::high_resolution_clock::now();
        
		MPI_Bcast(flat_B.data(), N * N, MPI_DOUBLE, 0 ,MPI_COMM_WORLD);
		
		std::vector<double> flat_A;
		if (rank == 0) {
			flat_A.resize(N * N);
			for (int t = 0; t < N; t++) {
				for (int i = 0; i < N; i++) {
					flat_A[t * N + i] = A[t][i];
				}
			}
		}
		
		std::vector<double> local_A(rows_per_proc * N);
		MPI_Scatter(flat_A.data(),rows_per_proc * N, MPI_DOUBLE,
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
			std::vector<std:vector<double>> C(N, std::vector<double>(N));
			for (int t = 0; t < N; t++) {
				for (int i = 0; i < N; i++) {
					C[t][i] = flat_C[t * N + i];
				}
			}
			
			write_matrix(argv[3], C);
		
			double ms = 
				std::chrono::duration<double, std::milli>(t2 - t1).count();
			long long ops = 2LL * N * N * N;

			write_matrix(argv[3], C);
			std::println("Кол-во процессов: {}", size);
			std::println("N = {}", N);
			std::println("Время = {:.3f} мс", ms);
			std::println("Объём задачи = {} операций", ops);
			std::println("Время на операцию = {:.6f} нс", ms * 1e6 / ops);
		}
	}
	
	catch (const std::exception& e) {
		std::cerr << "Ошибка: " << e.what() << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	MPI_Finalize();
	return 0;	
}
