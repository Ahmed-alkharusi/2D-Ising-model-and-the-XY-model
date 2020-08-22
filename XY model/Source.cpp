#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <cstdlib> //required for rand()
#include <ctime> //requiredd for time()
#include <cmath>
#include <iomanip>

const double pi{ 3.14159265359 };

std::string print_info(double& temperature, double& steps);
void xy_model_metropolis(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature, int random_spin);

void output_matrix(std::vector<std::vector<int>>& matrix, std::string&& file_name);

class IsingModel : public olc::PixelGameEngine
{
public:
	IsingModel()
	{
		sAppName = "2D XY model - Metropolis algorithm";
	}
	std::vector<std::vector<int>> lattice;
private:
	int x_shift = 2;
	int y_shift = 2;


	double temperature{0.5};
	unsigned int random_row = 0;
	unsigned int random_col = 0;
	int colour_map{ -1 };
	double steps{0};
public:

	bool OnUserCreate() override
	{
		srand(time(nullptr));

		for (unsigned int i{ 0 };i < lattice.size();i++) {
			for (unsigned int j{ 0 };j < lattice.size();j++) {
				lattice[i][j] = rand() % 1000;
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
				//XY_MODEL
				if (colour_map < 125)
					Draw(i, j, olc::Pixel(255, 0, 255));
				else if (colour_map < 250)
					Draw(i, j, olc::Pixel(255, 0, 0));
				else if (colour_map < 375)
					Draw(i, j, olc::Pixel(255, 127, 0));
				else if (colour_map < 500)
					Draw(i, j, olc::Pixel(255, 255, 0));
				else if (colour_map < 625)
					Draw(i, j, olc::Pixel(0, 255, 0));
				else if (colour_map < 750)
					Draw(i, j, olc::Pixel(0, 0, 255));
				else if (colour_map < 825)
					Draw(i, j, olc::Pixel(46, 43, 95));
				else if (colour_map < 1000)
					Draw(i, j, olc::Pixel(139, 0, 255));

			}
		}

			random_row = rand() % (lattice.size() - 2) + 1;// -2 since we are ignoring the edge
			random_col = rand() % (lattice.size() - 2) + 1;
			int random_spin = rand() % 1000;

			xy_model_metropolis(random_row, random_col, lattice, temperature, random_spin);


			if (GetKey(olc::Key::A).bHeld) temperature += 0.0001;
			if (GetKey(olc::Key::S).bHeld) {
				if (temperature > 0.0002)
					temperature -= 0.0001;
			}
			DrawString(113, 10, print_info(temperature, steps));
			steps += 1;

		return true;
	}
};

int main() {
	std::cout
		<< "=============================================================================\n"
		<< "2D XY model - Metropolis algorithm\n"
		<< "============================================================================\n"
		<< "Updated on Aug 16 2020\n"
		<< "@author: Ahmed Alkharusi\n"
		<< "I used the olcPixelGameEngine to generate the graphics.\n"
		<< "The 'olcPixelGameEngine.h' is a single header file that enables us to draw graphics.\n"
		<< "This is created by javidx9(OneLoneCoder).\n"
		<< "please download it from his repo\n"
		<< "https ://github.com/OneLoneCoder/olcPixelGameEngine" << std::endl;
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
std::string print_info(double& temperature, double& steps) {
	std::ostringstream oss;
	oss << "Press A/S\nto +/- T\n\n"
		<< std::setprecision(2) << std::left << "T = " << temperature << " K\n\n"
		<< "\n\n"
		<< std::left << "STEPS:\n\n" << std::setprecision(2) << steps << "\n\n"
		<< std::endl;
	std::string str = oss.str();
	return str;
}
void xy_model_metropolis(int row, int col, std::vector<std::vector<int>>& lattice, double& temperature, int random_spin) {
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
