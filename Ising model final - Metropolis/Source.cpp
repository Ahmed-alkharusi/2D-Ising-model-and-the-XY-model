/*
=============================================================================
 2D Ising model
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
// TAKE J = 1 = k_B

std::string print_info(double &t, double &steps);
void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name);
void metropolis(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature);
class IsingModel : public olc::PixelGameEngine
{
public:
	IsingModel()
	{
		sAppName = "2D Ising model - Metropolis algorithm";
	}
	std::vector<std::vector<int>> lattice;
private:
	int x_shift = 2;
	int y_shift = 2;
	double steps{ 0 };
	double temperature{ 0.5 };
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

	metropolis(random_row, random_col, lattice, temperature);


		if (GetKey(olc::Key::A).bHeld) temperature += 0.0001;
		if (GetKey(olc::Key::S).bHeld) {
			if (temperature > 0.0002)
				temperature -= 0.0001;
		}	
		DrawString(113, 10, print_info(temperature,steps));
		steps += 1;
		return true;
	}
};

int main() {
	std::cout
		<<"=============================================================================\n"
		<< "2D Ising model - Metropolis algorithm\n"
		<< "============================================================================\n"
		<< "Updated on Aug 16 2020\n"
		<< "@author: Ahmed Alkharusi\n"
		<< "I used the olcPixelGameEngine to generate the graphics.\n"
		<< "The 'olcPixelGameEngine.h' is a single header file that enables us to draw graphics.\n"
		<< "This is created by javidx9(OneLoneCoder).\n"
		<< "please download it from his repo\n"
		<< "https ://github.com/OneLoneCoder/olcPixelGameEngine" <<std::endl;

	IsingModel model;
	model.lattice = std::vector<std::vector<int>>(100, std::vector<int>(100));
	if (model.Construct(192, 108,10,10))
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
	oss << "Press A/S\nto +/- T\n\n" 
		<< std::setprecision(3) << std::left << "T= " << temperature << " K\n\n"
		<< "\n\n"
		<< std::left << "STEPS:\n\n" << std::setprecision(2) << steps << "\n\n"
		<< std::endl;
	std::string str = oss.str();
	return str;
}
void metropolis(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature) {
	double energy_difference = lattice[row][col] * (lattice[row][col + 1] + lattice[row][col - 1] + lattice[row + 1][col] + lattice[row - 1][col]);

	float random = (rand() % 1000) / 1000.0;
	if (0 > energy_difference) {
		lattice[row][col] = -lattice[row][col];
	}
	else if (random < std::exp(-(energy_difference) / temperature)) {
		lattice[row][col] = -lattice[row][col];
	}
}
