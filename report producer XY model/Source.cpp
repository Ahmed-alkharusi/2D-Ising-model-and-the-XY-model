/*
std::cout
	<<"=============================================================================\n"
	<< "2D Ising model - XY model report\n"
	<< "============================================================================\n"
	<< "Updated on Aug 16 2020\n"
	<< "@author: Ahmed Alkharusi\n"
	<< "I used the olcPixelGameEngine to generate the graphics.\n"
	<< "The 'olcPixelGameEngine.h' is a single header file that enables us to draw graphics.\n"
	<< "This is created by javidx9(OneLoneCoder).\n"
	<< "please download it from his repo\n"
	<< "https ://github.com/OneLoneCoder/olcPixelGameEngine" <<std::endl;
*/

#include <vector>
#include <cstdlib> //required for rand()
#include <ctime> //requiredd for time()
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
const double pi{ 3.14159265359 };
// TAKE J = 1 = k_B
void xy_model_metropolis(int row, int col, std::vector<std::vector<int>>& lattice, float& temperature, int random_spin);
void output_data(std::vector<float>& y, std::string&& file_name);
int hamiltonian(std::vector<std::vector<int>>& lattice);
int absolute_magnetization(std::vector<std::vector<int>>& lattice);
void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name);
int main() {
	std::vector<std::vector<int>> lattice = std::vector<std::vector<int>>(100, std::vector<int>(100));;
	std::vector<std::vector<int>> lattice_temp = lattice;
	int x_shift = 2;
	int y_shift = 2;
	int steps{ 0 };
	int equilibrium_steps{ 500000 };

	float temperature{ 0.001f };
	float temperature_increment{0.01f};
	float temperature_max{4};

	unsigned int random_row = 0;
	unsigned int random_col = 0;
	int colour_map{ -1 };

	std::vector<float>energies;
	std::vector<float>heat_capacity;
	std::vector<float>absolute_magnetization_vec;
	std::vector<float>temperatures;
	int sites_no{ static_cast<int>(lattice.size() * lattice.size()) };


	srand(time(nullptr));
	for (unsigned int i{ 0 };i < lattice.size();i++) {
		for (unsigned int j{ 0 };j < lattice.size();j++) {
			lattice[i][j] = rand() % 2;
			if (lattice[i][j] == 0)
				lattice[i][j] = -1;
		}
	}
	while (temperature < temperature_max) {
		random_row = rand() % (lattice.size() - 2) + 1;// -2 since we are ignoring the edge
		random_col = rand() % (lattice.size() - 2) + 1;
		int random_spin = rand() % 1000;
		xy_model_metropolis(random_row, random_col, lattice, temperature,random_spin);

		if (steps == equilibrium_steps) {
			energies.push_back(hamiltonian(lattice));
			(energies.size()>1) ? heat_capacity.push_back((energies[energies.size()-1]-energies[energies.size()-2]) / (temperature_increment)): heat_capacity.push_back(0);

			absolute_magnetization_vec.push_back(1.0 * absolute_magnetization(lattice) / sites_no);
			temperatures.push_back(temperature);
			steps = 0;

			temperature += temperature_increment;
		}
		steps += 1;
	}
	std::cout << "Writing files ..." << std::endl;
	output_data(heat_capacity,"results//heat_capacity.txt");
	output_data(absolute_magnetization_vec,"results//absolute_magnetization_vec.txt");
	output_data(temperatures, "results//temperatures.txt");
	output_data(energies,"results//energies.txt");
	output_matrix(lattice, "results//lattice.txt");
	return 0;
}
void output_data(std::vector<float> &y, std::string&& file_name) {
	std::ofstream output_file(file_name);
	for (size_t i{ 0 };i < y.size();i++) {
		output_file << y[i] << " ";
	}
}

int hamiltonian(std::vector<std::vector<int>>& lattice) {
	int sum2{ 0 };
	for (size_t i{ 1 };i < lattice.size() - 1;i++) {
		for (size_t j{ 1 };j < lattice.size() - 1;j++) {
			sum2 += lattice[i][j] * lattice[i + 1][j];
			sum2 += lattice[i][j] * lattice[i - 1][j];
			sum2 += lattice[i][j] * lattice[i][j + 1];
			sum2 += lattice[i][j] * lattice[i][j - 1];
		}
	}
	return sum2;
}
int absolute_magnetization(std::vector<std::vector<int>>& lattice) {
	int sum{ 0 };
	for (size_t i{ 1 };i < lattice.size() - 1;i++) {
		for (size_t j{ 1 };j < lattice.size() - 1;j++) {
			sum += lattice[i][j];
		}
	}
	return abs(sum);
}
void xy_model_metropolis(int row, int col, std::vector<std::vector<int>>& lattice, float& temperature, int random_spin) {
	double ratio = (2.0 * pi) / 1000.0;
	double energy_proposed = -(cos(ratio * (random_spin - lattice[row][col + 1])) + cos(ratio * (random_spin - lattice[row][col - 1])) + cos(ratio * (random_spin - lattice[row + 1][col])) + cos(ratio * (random_spin - lattice[row - 1][col])));
	double energy = -(cos(ratio * (lattice[row][col] - lattice[row][col + 1])) + cos(ratio * (lattice[row][col] - lattice[row][col - 1])) + cos(ratio * (lattice[row][col] - lattice[row + 1][col])) + cos(ratio * (lattice[row][col] - lattice[row - 1][col])));

	float random = (rand() % 1000) / 1000.0;

	if (energy_proposed < energy) {
		lattice[row][col] = random_spin;
	}
	else if (random < std::exp(-(energy_proposed - energy) / (temperature))) {
		lattice[row][col] = random_spin;
	}
}
void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name) {
	std::ofstream output_file(file_name);
	//here we are skipping the edges as they are not part of the results i.e. starting form 1 to matrix.size()-1
	for (size_t i{ 1 };i < matrix.size() - 1;i++) {
		for (size_t j{ 1 };j < (matrix.at(i)).size() - 1;j++) {
			output_file << (matrix.at(i)).at(j) << " ";
		}
		output_file << "\n";
	}
}
