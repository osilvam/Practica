#ifndef SIMFILES_HPP
#define SIMFILES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <ROBOTLIB>
#include "Fitness.hpp"
#include "CalcFunctions.hpp"

using namespace std;

class SimFiles
{
	ofstream file_results;
	ofstream file_fitness;
	ofstream file_champions;

public:

	SimFiles();
	~SimFiles();

	void addFileResults(Fitness * fitness, int generation, int population);
	void addFileFitness(Fitness * fitness, int generation);
	void addFileChampion(double fitness, int generation);
};

#endif
