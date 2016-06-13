#ifndef RIAR_HPP
#define RIAR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include <NEAT>
#include <ROBOTLIB>
#include "CalcFunctions.hpp"
#include "Fitness.hpp"
#include "SimFiles.hpp"
//#include <typeinfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace ANN_USM;

#define NX 32 // X image resolution
#define NY 32 // Y image resolution

//#define TIME_SIMULATION 10000 // milliseconds
#define TIME_SIMULATION 20000000 // microseconds
#define DELTA_TIME 50000 // microseconds

#endif
