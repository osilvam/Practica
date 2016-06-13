#include "RIAR.hpp"

/* V-REP implementation of an Autonomous Robot that avoid obstacles and advances without
preferred direction. The algotithm uses NEAT to evolve. */

int main(int argc, char* argv[])
{
	srand (time(0));    

    SimFiles * simfile = new SimFiles();
	Fitness * fitness = new Fitness();
	RobotVREP * vrep = new RobotVREP(true,20000);
    Retina * retina = new Retina();
    
    if(argc < 3)
	{
		cerr << "ERROR: The number of arguments is incorrect" << endl << "Enter:\targ_1 = user_definition_file\t arg_2 = genetic_encoding_file";
		return -1;
	}

	// ============= VREP INITIALIZATIONS ============= //
    
	Joint * rightWheel = new Joint((char*)"SCALE", (char*)"motor_der");
	Joint * leftWheel = new Joint((char*)"SCALE", (char*)"motor_izq");
    vrep->addJoint(rightWheel);
    vrep->addJoint(leftWheel);

    VisionSensor * visionSensor = new VisionSensor((char*)"VisionSensor");
    vrep->addVisionSensor(visionSensor);

    Object * centerDummy = new Object((char*)"modi_dummy");
    Object * Modi = new Object((char*)"MODI");
    vrep->addObject(centerDummy);
    vrep->addObject(Modi);

    CollisionObject * chasis = new CollisionObject((char*)"Collision81#");
    CollisionObject * rueda1 = new CollisionObject((char*)"Collision82#");
    CollisionObject * rueda2 = new CollisionObject((char*)"Collision83#");
    vrep->addCollisionObject(chasis);
    vrep->addCollisionObject(rueda1);
    vrep->addCollisionObject(rueda2);

    vector < Object * > cubes;

    // Set random position of Obstacles

    double y0 = -2;

    for(int cp_y = 0; cp_y < 9; cp_y++)
    {   
        double x0 = -2;

        for(int cp_x = 0; cp_x < 9; cp_x++)
        {            
            if(9*cp_y + cp_x != 40)
            {
                stringstream sstm1;
                sstm1 << "Obstacle" << 9*cp_y+cp_x<< "#";

                Object * obstacle = new Object((char*)sstm1.str().c_str());
                vrep->addObject(obstacle);

                double rand1 = rand()%201 - 100;
                double rand2 = rand()%201 - 100;

                vector < double > position;

                position.push_back(x0 + rand1/100*.20);
                position.push_back(y0 + rand2/100*.20);
                position.push_back(0.05);

                vrep->setObjectPosition(obstacle, position);

                cubes.push_back(obstacle);

            }

            x0 = x0 + 0.5;
        }

        y0 = y0 + 0.5;
    }

    // ================================================ //

	// ========== NEAT INITIALIZATIONS =========== //

	vector < double > output(2,0.0);
    vector < double > input(NX*NY+2,0.0);

    Population population(argv[1], argv[2], (char *)"NEAT_RIAR", (char *)"./NEAT_organisms");

	// ================================================ //
	
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.

	for(int g = 0; g < population.GENERATIONS; g++)
	{
		fitness->resetGenerationValues();

        int generationChampion = 0;
        double generationChampionFitness = 0.0;
     
		for(int p = 0; p < population.POPULATION_MAX; p++)
		{
            fitness->resetPopulationValues();

            int sim_time = 0;
            double rightVel = 0.0;
            double leftVel = 0.0;

            vrep->setJointTargetVelocity(rightWheel, 0.0);
            vrep->setJointTargetVelocity(leftWheel, 0.0);

            double rand1 = rand()%201 - 100;
            double rand2 = rand()%201 - 100;
            double rand3 = rand()%201 - 100;

            vector < double > position, orientation;

            position.push_back(rand1/100*.20);
            position.push_back(rand2/100*.20);
            position.push_back(0.02672);

            orientation.push_back(0);
            orientation.push_back(0);
            orientation.push_back(rand3/100*M_PI);
            
            vrep->setObjectPosition(Modi, position);
            vrep->setObjectOrientation(Modi, orientation);

            unsigned char * image;
            Mat frameRGB = Mat(NX, NY, CV_8UC3);
            Mat frameGRAY = Mat(NX, NY, CV_8UC1);

			stringstream message1, message2, video_name;			

            message1 << "Generation " << g << " Population " << p;
            vrep->addStatusbarMessage((char*)message1.str().c_str());

            video_name << "G" << g << "P" << p;
            vrep->changeVideoName((char *)video_name.str().c_str(), simx_opmode_oneshot_wait);

            vrep->startSimulation(simx_opmode_oneshot_wait);

            timeval tv1, tv2;
            int iter = 0;
			
            gettimeofday(&tv1, NULL);            

			while(sim_time < TIME_SIMULATION)
			{                
                image = vrep->getVisionSensorImage(visionSensor);  
                frameRGB.data = image;
                flip(frameRGB, frameRGB, 0);
                cvtColor(frameRGB,frameGRAY,CV_BGR2GRAY);
                
                Mat tmp = frameGRAY;
                Mat frame = tmp;

                tmp = retina->GetImg(frame);

                resize(tmp, frame, Size(0,0) , 6.0, 6.0, (int)INTER_NEAREST );
                //imshow( "Display window", frame );
                //waitKey(10);

                for(int i = 0; i < NY; i++)
                {
                    for(int j = 0;j < NX; j++)
                    {
                        input.at(i*NX + j) = (double)frame.at<uchar>(i,j)/255*2-1;
                    }
                }
                
                input.at(NX*NY) = (double)rightVel/8;
                input.at(NX*NY + 1) = (double)leftVel/8;

                output = population.organisms.at(p).eval(input);

                rightVel = output.at(0) + rightVel;
                leftVel = output.at(1) + leftVel;

                if(rightVel > 8) rightVel = 8;
                else if(rightVel < -8) rightVel = -8;
                if(leftVel > 8) leftVel = 8;
                else if(leftVel < -8) leftVel = -8;

                vrep->setJointTargetVelocity(rightWheel,-rightVel);
                vrep->setJointTargetVelocity(leftWheel,leftVel);

                fitness->measuringValues(vrep->getObjectPosition(centerDummy), rightVel, leftVel,  vrep->checkAllCollisions());

                iter++;

                usleep(DELTA_TIME);
                sim_time += DELTA_TIME;
			}            

			vrep->stopSimulation(simx_opmode_oneshot_wait);

            gettimeofday(&tv2, NULL);
            long int simulationtime = ((tv2.tv_sec - tv1.tv_sec)*1000000L + tv2.tv_usec) - tv1.tv_usec;            
						
            population.organisms.at(p).fitness = fitness->calculateFitness();
            simfile->addFileResults(fitness, g, p);

            stringstream organism_filename;
            organism_filename << "NEAT_organisms/G" << g << "P" << p << ".txt";
            population.organisms.at(p).save((char *)organism_filename.str().c_str());

			clog << "=======  G" << g << " P" << p << "  =======  " << endl;
            clog << "Fitness:\t" << fitness->getFitness() << endl;
            clog << "Tiempo de simulación:\t" << (double)simulationtime/1000000 << endl;
            clog <<"# iteraciones:\t" << iter << endl << endl;

            message2 << "FITNESS : " << fitness->getFitness();
            vrep->addStatusbarMessage((char*)message2.str().c_str());

            if(generationChampionFitness < fitness->getFitness())
            {
                generationChampion = p;
                generationChampionFitness = fitness->getFitness();
            }
		}

        stringstream generation_champion_filename;
        generation_champion_filename << "NEAT_organisms/Champion_G" << g << ".txt";
        population.organisms.at(generationChampion).save((char *)generation_champion_filename.str().c_str());

        simfile->addFileChampion(generationChampionFitness,g);

		population.epoch();

        simfile->addFileFitness(fitness, g);
	}
	

    clog << "Fitness champion: " << population.fitness_champion << "\n\n"<< endl;

	delete(vrep);
	delete(simfile);
	delete(fitness);
    delete(retina);
	
	return(0);
}
