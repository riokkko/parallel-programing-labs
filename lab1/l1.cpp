import std;
import maker;

int main (int argc, char** argv) { 
	begining();
	if (argc != 4) {
        std::println("Использование: {} A.txt B.txt C.txt", argv[0]);
        return 1;
    }
	try {
		std::vector<std::vector<double>> A, B;
		set_matrix(argv[1], A);
        set_matrix(argv[2], B);
		can_multyply(A, B);

		auto t1 = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<double>> C = multyply_matrix(A, B);
        auto t2 = std::chrono::high_resolution_clock::now();

        double ms = 
			std::chrono::duration<double, std::milli>(t2 - t1).count();
        size_t n = A.size();
        long long ops = 2LL * n * n * n;

		write_matrix(argv[3], C);
		std::println("N = {}", n);
		std::println("Время = {:.3f} мс", ms);
		std::println("Объём задачи = {} операций", ops);
		std::println("Время на операцию = {:.6f} нс", ms * 1e6 / ops);
	}
	
	catch (const std::exception& e) {
		std::cerr << "Ошибка: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;	
}
