export module maker;
import std;


export void begining () {
	std::println("-----------------------");
	std::println("   ЛР №: 1");
	std::println("Группа: 6211");
	std::println("  Автор: Острицова Екатерина");
	std::println("-----------------------");
}

export void set_matrix (const std::string& f, 
std::vector<std::vector<double>>& matrix) {
	std::ifstream file(f);
	std::string line;
	if (!file.is_open()) {
		throw std::runtime_error
		(std::format("Ошибка открытия файла {}!", f));
	   }
	matrix.clear();
	while (std::getline(file,line)) {
		if(line.empty()) continue;
		std::stringstream s(line);
		double value;
		std::vector<double> row;
		while (s >> value) row.push_back(value);
		if (!row.empty()) matrix.push_back(row);
	}
}

export std::vector<std::vector<double>> 
multyply_matrix(const std::vector<std::vector<double>>& matrix1,
const std::vector<std::vector<double>>& matrix2) {
	size_t row1 = matrix1.size();
	size_t colow = matrix1[0].size();
	size_t row2 = matrix2[0].size();
	
	std::vector<std::vector<double>> result 
		(row1, std::vector<double>(row2,0.0));
	for (size_t i = 0; i < row1; i++) {
		for (size_t j = 0; j < row2; j++) {
			for (size_t t = 0; t < colow; t++) {
				result[i][j] += matrix1[i][t] * matrix2[t][j];
			}
		}
	}
	return result;
}

export bool can_multyply(const std::vector<std::vector<double>>& matrix1,
const std::vector<std::vector<double>>& matrix2) {
	if (matrix1.empty() || matrix2.empty())
		throw std::runtime_error ("Матрицы нельзя перемножить");
	return matrix1[0].size() == matrix2.size();
}

export void write_matrix(const std::string& f, 
const std::vector<std::vector<double>>& matrix) {
	std::ofstream file(f);
	if (!file.is_open()) {
		throw std::runtime_error
			(std::format("Ошибка открытия файла {}!", f));
	}
	for (const std::vector<double> row: matrix) {
		for (size_t t = 0; t < row.size(); t++) {
			file << row[t];
			if (t + 1 < row.size()) file << ' ';
		}
		file << '\n';
	}
}
	
