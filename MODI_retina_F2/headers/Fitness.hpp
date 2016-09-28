#ifndef FITNESS_HPP
#define FITNESS_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include "CalcFunctions.hpp"

using namespace std;

#define FAILED_FITNESS 0.00001
#define FITNESS_BASE 0.00001

class Fitness
{
	double fitness;
	double distance;
	vector < vector < double > > robot_position;
	vector < double > robot_tail;	
	vector < double > robot_rightVel;
	vector < double > robot_leftVel;
	vector < double > generation_fitness;

public:

	Fitness();
	~Fitness();

	void measuringValues(vector < double > position, double rightVel, double leftVel, bool collision);
	double calculateFitness();
	void resetPopulationValues();
	void resetGenerationValues();
	double getFrecuency();
	double getFitness();
	double getDistance();
	vector < double > getGenerationFitness();

};

#endif
