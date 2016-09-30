#ifndef FITNESS_CPP
#define FITNESS_CPP

#include "Fitness.hpp"

Fitness::Fitness()
{
	fitness = FAILED_FITNESS;
	distance = FITNESS_BASE;
}

Fitness::~Fitness()
{

}

void Fitness::measuringValues(vector < double > position, double rightVel, double leftVel)
{
	double aux1;

    if( (rightVel > 0.0) && (leftVel > 0.0) ) aux1 = 1;
    else aux1 = 0;

	robot_position.push_back(position);
	robot_tail.push_back(aux1);
	robot_rightVel.push_back(rightVel);
	robot_leftVel.push_back(leftVel);
}

double Fitness::calculateFitness()
{
	if(stdDesviation(robot_rightVel) < 0.5 && stdDesviation(robot_leftVel) < 0.5)
	{
		cerr << "FITNESS ERROR:\tNAVIGATION WITHOUT DIRECTION CHANGES" << endl;
		fitness = FAILED_FITNESS;
	}
	else
	{
	    for(int i = 1; i < (int)robot_position.size(); i++)
	    {

	        double x0 = robot_position.at(i - 1).at(0);
	        double x1 = robot_position.at(i).at(0);
	        double y0 = robot_position.at(i - 1).at(1);
	        double y1 = robot_position.at(i).at(1);
	        double tail = robot_tail.at(i - 1);

	        distance = distance + tail*sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));

	    }
	    fitness = distance;//10/(1 + exp(3*(-distance + 2)));
	}

    generation_fitness.push_back(fitness);

	return fitness;
}

void Fitness::resetPopulationValues()
{
	fitness = FAILED_FITNESS;
	distance = FITNESS_BASE;
	robot_position.clear();
	robot_tail.clear();
	robot_rightVel.clear();
	robot_leftVel.clear();
}

void Fitness::resetGenerationValues()
{
	generation_fitness.clear();
}


double Fitness::getFitness()
{
	return fitness;
}

double Fitness::getDistance()
{
	return distance;
}

vector < double > Fitness::getGenerationFitness()
{
	return generation_fitness;
}


#endif
