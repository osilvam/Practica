#ifndef CALCFUNCTIONS_HPP
#define CALCFUNCTIONS_HPP

#include <vector>
#include <cmath>

using namespace std;

double mean(vector < double > numbers);
double mean(vector < int > numbers);
double var(vector < double > numbers);
double var(vector < int > numbers);
double stdDesviation(vector < double > numbers);
double stdDesviation(vector < int > numbers);
double min(vector < double > numbers, int * index = nullptr);
int min(vector < int > numbers, int * index = nullptr);
double max(vector < double > numbers, int * index = nullptr);
int max(vector < int > numbers, int * index = nullptr);

#endif