#ifndef SIMFILES_CPP
#define SIMFILES_CPP

#include "SimFiles.hpp"

SimFiles::SimFiles()
{
	file_fitness.open("fitness.txt");
	file_results.open("results.txt");
	file_champions.open("champions.txt");
}

SimFiles::~SimFiles()
{
	file_fitness.close();
	file_results.close();
	file_champions.close();
}

void SimFiles::addFileResults(Fitness * fitness, int generation, int population)
{
	file_results << "G" << generation << "\tP" << population << "\tFitness:\t" << fitness->getFitness() << endl;
}

void SimFiles::addFileFitness(Fitness * fitness, int generation)
{
	double average = mean(fitness->getGenerationFitness());
	double stddesv = stdDesviation(fitness->getGenerationFitness());

	file_fitness << generation << "\t" << average << "\t" << stddesv << endl;
}

void SimFiles::addFileChampion(double fitness, int generation)
{
	file_champions << generation << "\t" << fitness << endl;
}


#endif
