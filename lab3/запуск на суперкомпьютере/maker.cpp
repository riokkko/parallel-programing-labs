#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

void begining();
void set_matrix(const std::string& f, std::vector<std::vector<double>>& matrix);
void write_matrix(const std::string& f, const std::vector<std::vector<double>>& matrix);
bool can_multyply(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2);

void begining () {
	std::cout << "-----------------------";
	std::cout << "   ЛР №: 3";
	std::cout << "Группа: 6211";
	std::cout << "  Автор: Острицова Екатерина";
	std::cout << "-----------------------";
}

void set_matrix (const std::string& f, 
std::vector<std::vector<double>>& matrix) {
	std::ifstream file(f);
	std::string line;
	if (!file.is_open()) {
		throw std::runtime_error
		("Ошибка открытия файла " + f);
	   }
	matrix.clear();
	while (std::getline(file, line)) {
        	if (line.empty()) continue;
        	std::stringstream s(line);
        	double value;
        	std::vector<double> row;
        	while (s >> value) row.push_back(value);
        	if (!row.empty()) matrix.push_back(row);
    	}
}

bool can_multyply(const std::vector<std::vector<double>>& matrix1,
const std::vector<std::vector<double>>& matrix2) {
	if (matrix1.empty() || matrix2.empty())
		throw std::runtime_error ("Матрицы нельзя перемножить");
	return matrix1[0].size() == matrix2.size();
}

void write_matrix(const std::string& f, const std::vector<std::vector<double>>& matrix) {
    std::ofstream file(f);
    if (!file.is_open()) throw std::runtime_error("Ошибка открытия файла " + f);
    for (size_t t = 0; t < matrix.size(); t++) {
        for (size_t i = 0; i < matrix[t].size(); i++) {
            file << matrix[t][i];
            if (i + 1 < matrix[t].size()) file << ' ';
        }
        file << '\n';
    }
}	