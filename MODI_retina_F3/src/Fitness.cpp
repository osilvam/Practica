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
	robot_position.push_back(position);
	robot_rightVel.push_back(rightVel);
	robot_leftVel.push_back(leftVel);
}

double Fitness::calculateFitness()
{
	vector < double > velDifference;
	
	for(int i = 0; i < (int)robot_rightVel.size(); i++)
		velDifference.push_back(abs(robot_rightVel.at(i)-robot_leftVel.at(i)));

	if(stdDesviation(velDifference) < 0.1)
	{
		cerr << "FITNESS ERROR:\tNAVIGATION WITHOUT DIRECTION CHANGES" << endl;
		fitness = FAILED_FITNESS;
	}
	else
	{
	    double x0 = robot_position.front().at(0);
        double x1 = robot_position.back().at(0);
        double y0 = robot_position.front().at(1);
        double y1 = robot_position.back().at(1);

        distance = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));

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
