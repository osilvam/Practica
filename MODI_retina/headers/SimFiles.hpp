#ifndef SIMFILES_HPP
#define SIMFILES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include "CalcFunctions.hpp"

using namespace std;

class SimFiles
{
	ofstream file_results;
	ofstream file_fitness;
	ofstream file_champions;

	ofstream file_robotPosition;
	ofstream file_robotMotorVelocity;

public:

	SimFiles();
	~SimFiles();

	void addFileResults(double fitness, int generation, int population);
	void addFileFitness(vector < double > fitness, int generation);
	void addFileChampion(double fitness, int generation);

	void openRobotMovementFile(int generation, int population);
	void closeRobotMovementFile();
	void addRobotMovementFile(vector < double > position, double orientation);

	void openRobotMotorVelocityFile(int generation, int population);
	void closeRobotMotorVelocityFile();
	void addRobotMotorVelocityFile(double rightVel, double leftVel);
};

#endif
