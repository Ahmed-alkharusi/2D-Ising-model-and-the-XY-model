/*
=============================================================================
 2D Ising model -Wolff
 =============================================================================
Updated on Aug 22 2020
@author: Ahmed Alkharusi
I used the olcPixelGameEngine to generate the graphics.
The "olcPixelGameEngine.h" is a single header file that enables us to draw graphics.
This is created by javidx9 (OneLoneCoder).
please download it from his repo
https://github.com/OneLoneCoder/olcPixelGameEngine
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <cstdlib> //required for rand()
#include <ctime> //requiredd for time()
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
std::string print_info(double &t, double &steps);
void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name);
void wolff(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature);
class IsingModel : public olc::PixelGameEngine
{
public:
	IsingModel()
	{
		sAppName = "2D Ising model - Wolff algorithm";
	}
	std::vector<std::vector<int>> lattice;
private:
	int x_shift = 2;
	int y_shift = 2;
	double steps{ 0 };
	double temperature{ 3.5 };
	unsigned int random_row = 0;
	unsigned int random_col = 0;
	int colour_map{ -1 };
public:

	bool OnUserCreate() override
	{
		srand(time(nullptr));

		for (unsigned int i{ 0 };i < lattice.size();i++) {
			for (unsigned int j{ 0 };j < lattice.size();j++) {
				lattice[i][j] = rand() % 2;
				if (lattice[i][j] == 0)
					lattice[i][j] = -1;
			}
		}

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
	Clear(olc::BLACK);
		
	for (unsigned int i{ 1 };i < lattice.size() - 1;i++) {
		for (unsigned int j{ 1 };j < lattice.size() - 1;j++) {
			colour_map = lattice[i][j];
			Draw(i + x_shift, j + y_shift, (colour_map == -1) ? olc::Pixel(0, 0, 0) : olc::Pixel(255, 255, 255));
		}
	}

	random_row = rand() % (lattice.size() - 2) + 1;// -2 since we are ignoring the edge
	random_col = rand() % (lattice.size() - 2) + 1;
	
	wolff(random_row, random_col, lattice, temperature);
		if (GetKey(olc::Key::A).bHeld) temperature += 0.0001;
		if (GetKey(olc::Key::S).bHeld) {
			if (temperature > 2.4)
				temperature -= 0.0001;
		}
		DrawString(226, 10, print_info(temperature,steps));

		steps += 1;
		return true;
	}
};

int main() {
	std::cout
		<<"=============================================================================\n"
		<< "2D Ising model - Wolff algorithm\n"
		<< "============================================================================\n"
		<< "Updated on Aug 16 2020\n"
		<< "@author: Ahmed Alkharusi\n"
		<< "I used the olcPixelGameEngine to generate the graphics.\n"
		<< "The 'olcPixelGameEngine.h' is a single header file that enables us to draw graphics.\n"
		<< "This is created by javidx9(OneLoneCoder).\n"
		<< "please download it from his repo\n"
		<< "https ://github.com/OneLoneCoder/olcPixelGameEngine" <<std::endl;

	IsingModel model;
	model.lattice = std::vector<std::vector<int>>(200, std::vector<int>(200));
	if (model.Construct(384, 216,5,5))
		model.Start();
	//output_matrix(model.lattice, "results.txt");
	return 0;
}
void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name) {
	std::ofstream output_file(file_name);
	//here we are skipping the edges as they are not part of the results i.e. starting form 1 to matrix.size()-1
	for (size_t i{ 1 };i < matrix.size()-1;i++) {
		for (size_t j{ 1 };j < (matrix.at(i)).size()-1;j++) {
			output_file << (matrix.at(i)).at(j) << " ";
		}
		output_file << "\n";
	}
}
std::string print_info(double &temperature, double &steps) {
	std::ostringstream oss;
	oss << "Press A/S to +/- T\n\n" 
		<< std::setprecision(3) << std::left << "T = " << temperature << " K\n\n"
		<< "\n\n"
		<< std::left << "STEPS:\n\n" << std::setprecision(2) << steps << "\n\n"
		<< std::endl;
	std::string str = oss.str();
	return str;
}
void wolff(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature) {
	float random;
	int seed_spin = lattice[row][col];

	std::vector<std::vector<int>>pocket;
	pocket.emplace_back(std::initializer_list<int>{row, col});

	std::vector<std::vector<int>>pocket_temp;

	while (!(pocket.empty())) {

		pocket_temp = pocket;
		pocket.clear();
		for (auto& vec_t : pocket_temp) {
			if (seed_spin == lattice[vec_t[0]][vec_t[1] + 1] && vec_t[1] + 1 < lattice.size() - 1 && vec_t[0] > 0) {
				random = (rand() % 1000) / 1000.0;
				if (random < (1 - std::exp(-2.0 / temperature))) {
					pocket.emplace_back(std::initializer_list<int>{vec_t[0], vec_t[1] + 1});
					lattice[vec_t[0]][vec_t[1] + 1] *= -1;
				}
			}
			if (seed_spin == lattice[vec_t[0]][vec_t[1] - 1] && vec_t[0] > 0 && vec_t[1] - 1 > 0) {
				random = (rand() % 1000) / 1000.0;
				if (random < (1 - std::exp(-2.0 / temperature))) {
					pocket.emplace_back(std::initializer_list<int>{vec_t[0], vec_t[1] - 1});
					lattice[vec_t[0]][vec_t[1] - 1] *= -1;
				}
			}
			if (seed_spin == lattice[vec_t[0] + 1][vec_t[1]] && vec_t[0] + 1 < lattice.size() - 1 && vec_t[1] > 0) {
				random = (rand() % 1000) / 1000.0;
				if (random < (1 - std::exp(-2.0 / temperature))) {
					pocket.emplace_back(std::initializer_list<int>{vec_t[0] + 1, vec_t[1] });
					lattice[vec_t[0] + 1][vec_t[1]] *= -1;
				}
			}
			if (seed_spin == lattice[vec_t[0] - 1][vec_t[1]] && vec_t[0] - 1 > 0 && vec_t[1] > 1) {
				random = (rand() % 1000) / 1000.0;
				if (random < (1 - std::exp(-2.0 / temperature))) {
					pocket.emplace_back(std::initializer_list<int>{vec_t[0] - 1, vec_t[1] });
					lattice[vec_t[0] - 1][vec_t[1]] *= -1;
				}
			}
		}
	}

}