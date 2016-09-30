#ifndef SIMFILES_CPP
#define SIMFILES_CPP

#include "SimFiles.hpp"

SimFiles::SimFiles()
{	
	if(system("mkdir -p simulation_files") == -1)
	{
		cerr << "SIMFILES ERROR:\tFailed to create folder 'simulation_files'" << endl;
	}

	if(system("mkdir -p simulation_files/movement") == -1)
	{
		cerr << "SIMFILES ERROR:\tFailed to create folder 'simulation_files/movement'" << endl;
	}

	if(system("rm -f simulation_files/movement/*") == -1)
	{
		cerr << "SIMFILES ERROR:\tFailed to remove files inside of 'simulation_files/movement'" << endl;
	}

	if(system("mkdir -p simulation_files/motorVelocity") == -1)
	{
		cerr << "SIMFILES ERROR:\tFailed to create folder 'simulation_files/motorVelocity'" << endl;
	}

	if(system("rm -f simulation_files/motorVelocity/*") == -1)
	{
		cerr << "SIMFILES ERROR:\tFailed to remove files inside of 'simulation_files/motorVelocity'" << endl;
	}

	file_fitness.open("simulation_files/fitness.txt");
	file_results.open("simulation_files/results.txt");
	file_champions.open("simulation_files/champions.txt");

	file_fitness << "Generation\tFitness Mean\tFitness Stddesviation" << endl;
	file_results << "Generation\tPopulation\tFitness" << endl;
	file_champions << "Generation\tPopulation\tFitness" << endl;
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

void SimFiles::addFileChampion(double fitness, int generation, int population)
{
	file_champions << generation << "\t" << population << "\t" << fitness << endl;
}




void SimFiles::openRobotMovementFile(int generation, int population)
{
	stringstream file_name;
	file_name << "simulation_files/movement/movement_G" << generation << "P" << population << ".txt";
	file_robotPosition.open((char*)file_name.str().c_str());

	file_robotPosition << "Time\tPosition in X\tPosition in Y\tOrientation" << endl;
}

void SimFiles::closeRobotMovementFile()
{
	file_robotPosition.close();
}

void SimFiles::addRobotMovementFile(double sim_time, vector < double > position, double orientation)
{
	file_robotPosition << sim_time << "\t" << position.at(0) << "\t" << position.at(1) << "\t" << orientation << endl;
}





void SimFiles::openRobotMotorVelocityFile(int generation, int population)
{
	stringstream file_name;
	file_name << "simulation_files/motorVelocity/motorVelocity_G" << generation << "P" << population << ".txt";
	file_robotMotorVelocity.open((char*)file_name.str().c_str());

	file_robotMotorVelocity << "Time\tRight Velocity\tLeft Velocity" << endl;
}

void SimFiles::closeRobotMotorVelocityFile()
{
	file_robotMotorVelocity.close();
}

void SimFiles::addRobotMotorVelocityFile(double sim_time, double rightVel, double leftVel)
{
	file_robotMotorVelocity << sim_time << "\t" << rightVel << "\t" << leftVel << endl;
}



#endif
