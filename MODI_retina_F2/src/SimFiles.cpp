#ifndef SIMFILES_CPP
#define SIMFILES_CPP

#include "SimFiles.hpp"

SimFiles::SimFiles()
{
	file_fitness.open("fitness.txt");
	file_results.open("results.txt");
	file_champions.open("champions.txt");

	file_fitness << "Generation\tPopulation\tFitness" << endl;
	file_results << "Generation\tFitness Mean\tFitness Stddesviation" << endl;
	file_champions << "Generation\tFitness" << endl;
}

SimFiles::~SimFiles()
{
	file_fitness.close();
	file_results.close();
	file_champions.close();
}

void SimFiles::addFileResults(double fitness, int generation, int population)
{
	file_results << generation << "\t" << population << "\t" << fitness << endl;
}

void SimFiles::addFileFitness(vector < double > fitness, int generation)
{
	double average = mean(fitness);
	double stddesv = stdDesviation(fitness);

	file_fitness << generation << "\t" << average << "\t" << stddesv << endl;
}

void SimFiles::addFileChampion(double fitness, int generation)
{
	file_champions << generation << "\t" << fitness << endl;
}




void SimFiles::openRobotMovementFile(int generation, int population)
{
	stringstream file_name;
	file_name << "movement_G" << generation << "P" << population << ".txt";
	file_robotPosition.open((char*)file_name.str().c_str());

	file_robotPosition << "Position in X\tPosition in Y\tOrientation" << endl;
}

void SimFiles::closeRobotMovementFile()
{
	file_robotPosition.close();
}

void SimFiles::addRobotMovementFile(vector < double > position, double orientation)
{
	file_robotPosition << position.at(0) << "\t" << position.at(1) << "\t" << orientation << endl;
}





void SimFiles::openRobotMotorVelocityFile(int generation, int population)
{
	stringstream file_name;
	file_name << "motorVelocity" << generation << "P" << population << ".txt";
	file_robotMotorVelocity.open((char*)file_name.str().c_str());

	file_robotMotorVelocity << "Right Velocity\tLeft Velocity" << endl;
}

void SimFiles::closeRobotMotorVelocityFile()
{
	file_robotMotorVelocity.close();
}

void SimFiles::addRobotMotorVelocityFile(double rightVel, double leftVel)
{
	file_robotMotorVelocity << rightVel << "\t" << leftVel << endl;
}



#endif
