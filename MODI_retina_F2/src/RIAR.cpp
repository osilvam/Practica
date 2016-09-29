#include "RIAR.hpp"

/* V-REP implementation of an Autonomous Robot that avoid obstacles and advances without
preferred direction. The algotithm uses NEAT to evolve. */

int main(int argc, char* argv[])
{
    srand (time(0));    

    if(argc < 4)
    {
        cerr << "ERROR: The number of arguments is incorrect" << endl << "Enter:\targ_1 = user_definition_file\t arg_2 = genetic_encoding_file\t arg_3 = port_number";
        return -1;
    }

    if(system((char*)"mkdir -p NEAT_organisms") == -1)
    {
        cerr << "TRAIN ERROR:\tFailed to create folder 'NEAT_organisms'" << endl;
    }

    if(system("rm -f NEAT_organisms/*") == -1)
    {
        cerr << "TRAIN ERROR:\tFailed to remove files inside of 'NEAT_organisms'" << endl;
    }

    SimFiles * simfile = new SimFiles();
    Fitness * fitness = new Fitness();
    RobotVREP * vrep = new RobotVREP(false, atoi(argv[3]));    
    Retina * retina = new Retina(); 

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
    
    CollisionObject * chasis = new CollisionObject((char*)"Collision_MODI_1#");
    CollisionObject * rueda1 = new CollisionObject((char*)"Collision_MODI_2#");
    CollisionObject * rueda2 = new CollisionObject((char*)"Collision_MODI_3#");
    vrep->addCollisionObject(chasis);
    vrep->addCollisionObject(rueda1);
    vrep->addCollisionObject(rueda2);
    vector < CollisionObject * > structure = {chasis, rueda1, rueda2};

    vector < Object * > cubes;

    // Set random position of Obstacles

    double y0 = -2;

    for(int cp_y = 0; cp_y < 9; cp_y++)
    {   
        double x0 = -2 + 0.25*(cp_y%2);

        for(int cp_x = 0; cp_x < 8 + (cp_y + 1)%2; cp_x++)
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

                position.push_back(x0 + rand1/100*.10);
                position.push_back(y0 + rand2/100*.10);
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

    int finalChampionGeneration = 0;
    int finalChampionPopulation = 0;
    double finalChampionFitness = 0.0;

    for(int g = 0; g < population.GENERATIONS; g++)
    {
        fitness->resetGenerationValues();

        int generationChampionPopulation = 0;
        double generationChampionFitness = 0.0;
     
        for(int p = 0; p < population.POPULATION_MAX; p++)
        {
            fitness->resetPopulationValues();

            int sim_time = 0;
            bool flag = true;
            int flag_times = 0;
            double rightVel = 0.0;
            double leftVel = 0.0;

            vrep->setJointTargetVelocity(rightWheel, 0.0);
            vrep->setJointTargetVelocity(leftWheel, 0.0);

            double rand1 = rand()%201 - 100;
            double rand2 = rand()%201 - 100;
            double rand3 = rand()%201 - 100;

            vector < double > position, orientation;

            position.push_back(rand1/100*.10);
            position.push_back(rand2/100*.10);
            position.push_back(0.03011);

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
            //vrep->changeVideoName((char *)video_name.str().c_str(), simx_opmode_oneshot_wait);

            simfile->openRobotMovementFile(g, p);
            simfile->openRobotMotorVelocityFile(g, p);
            
            clog << "=======  G" << g << " P" << p << "  =======  " << endl;

            vrep->startSimulation(simx_opmode_oneshot_wait);

            timeval tv1, tv2;       
            gettimeofday(&tv1, NULL);            

            while(sim_time < TIME_SIMULATION && flag)
            {            
                image = vrep->getVisionSensorImage(visionSensor);  
                frameRGB.data = image;
                flip(frameRGB, frameRGB, 0);
                cvtColor(frameRGB,frameGRAY,CV_BGR2GRAY);
                
                Mat tmp = frameGRAY;
                Mat frame = tmp;

                tmp = retina->GetImg(frame);

                resize(tmp, frame, Size(0,0) , 6.0, 6.0, (int)INTER_NEAREST );
                imshow( "Display window", frame );
                waitKey(10);

                for(int i = 0; i < NY; i++)
                {
                    for(int j = 0;j < NX; j++)
                    {
                        input.at(i*NX + j) = (double)frame.at<uchar>(i,j)/255*2-1;
                    }
                }
                
                input.at(NX*NY) = (double)((2.0/(MAX_VEL - MIN_VEL))*(rightVel - MIN_VEL) - 1.0);
                input.at(NX*NY + 1) = (double)((2.0/(MAX_VEL - MIN_VEL))*(leftVel - MIN_VEL) - 1.0);

                output = population.organisms.at(p).eval(input);

                rightVel = output.at(0) + rightVel;
                leftVel = output.at(1) + leftVel;

                if(rightVel > MAX_VEL) rightVel = MAX_VEL;
                else if(rightVel < MIN_VEL) rightVel = MIN_VEL;
                if(leftVel > MAX_VEL) leftVel = MAX_VEL;
                else if(leftVel < MIN_VEL) leftVel = MIN_VEL;

                vrep->setJointTargetVelocity(rightWheel,-rightVel);
                vrep->setJointTargetVelocity(leftWheel,leftVel);                                              

                if(sim_time > TIME_INIT_MEASURING)
                {
                    position = vrep->getObjectPosition(centerDummy);
                    orientation = vrep->getObjectOrientation(centerDummy);

                    simfile->addRobotMovementFile((double)sim_time/1000000.0, position, orientation.at(2));
                    simfile->addRobotMotorVelocityFile((double)sim_time/1000000.0, rightVel, leftVel);

                    fitness->measuringValues(position, rightVel, leftVel, vrep->readCollision(structure));

                    if (abs(orientation.at(0)) > 0.78 || abs(orientation.at(1)) > 0.78)
                    {
                        flag_times++;
                        if(flag_times > 10) flag = false;
                    }else
                        flag_times = 0;
                }                         

                usleep(DELTA_TIME - EXECUTION_TIME);
                sim_time += DELTA_TIME;
            }            

            vrep->stopSimulation(simx_opmode_oneshot_wait);

            gettimeofday(&tv2, NULL);
            long int simulationtime = ((tv2.tv_sec - tv1.tv_sec)*1000000L + tv2.tv_usec) - tv1.tv_usec;   

            simfile->closeRobotMovementFile();
            simfile->closeRobotMotorVelocityFile();  

            if (flag)
            {                
                population.organisms.at(p).fitness = fitness->calculateFitness();             
                simfile->addFileResults(fitness->getFitness(), g, p);

                clog << "Fitness:\t" << fitness->getFitness() << endl;
                clog << "Distance:\t" << fitness->getDistance() << endl;
                clog << "Tiempo de simulación:\t" << (double)simulationtime/1000000 << endl;
                clog << endl;            

                message2 << "FITNESS : " << fitness->getFitness();
                vrep->addStatusbarMessage((char*)message2.str().c_str());

                if(generationChampionFitness < fitness->getFitness())
                {
                    generationChampionPopulation = p;
                    generationChampionFitness = fitness->getFitness();
                }
            }
            else
            {
                clog << "OVERTURNING! The simulation has stopped" << endl;
                population.organisms.at(p).fitness = FAILED_FITNESS;
            }                
        }

        simfile->addFileChampion(generationChampionFitness, g, generationChampionPopulation);
        simfile->addFileFitness(fitness->getGenerationFitness(), g);        

        //////////////////////////// SAVE CHAMPION FILES /////////////////////////////////

        stringstream generation_champion_filename;
        generation_champion_filename << "NEAT_organisms/Champion_G" << g << "P" << generationChampionPopulation << ".txt";
        population.organisms.at(generationChampionPopulation).save((char *)generation_champion_filename.str().c_str());

        stringstream cp_gen_champion_movement, cp_gen_champion_motorVelocity;

        cp_gen_champion_movement << "cp simulation_files/movement/movement_G" << g << "P" << generationChampionPopulation << ".txt ./simulation_files/movement/Champion_G" << g << "P" << generationChampionPopulation << ".txt";
        cp_gen_champion_motorVelocity << "cp simulation_files/motorVelocity/motorVelocity_G" << g << "P" << generationChampionPopulation << ".txt ./simulation_files/motorVelocity/Champion_G" << g << "P" << generationChampionPopulation << ".txt";

        if(system((char*)cp_gen_champion_movement.str().c_str()) == -1)
        {
            cerr << "TRAIN ERROR:\tFailed to copy the Champion movement File" << endl;
        }
        else
        {
            if(system("rm -f ./simulation_files/movement/movement_G*.txt") == -1)
            {
                cerr << "TRAIN ERROR:\tFailed to remove useless files" << endl;
            }
        }

        if(system((char*)cp_gen_champion_motorVelocity.str().c_str()) == -1)
        {
            cerr << "TRAIN ERROR:\tFailed to copy the Champion motor velocity File" << endl;
        }
        else
        {
            if(system("rm -f ./simulation_files/motorVelocity/motorVelocity_G*.txt") == -1)
            {
                cerr << "TRAIN ERROR:\tFailed to remove useless files" << endl;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////

        population.epoch();        

        if(finalChampionFitness < generationChampionFitness)
        {
            finalChampionGeneration = g;
            finalChampionPopulation = generationChampionPopulation;
            finalChampionFitness = generationChampionFitness;
        }
    }

    //////////////////////////// SAVE CHAMPION FILES /////////////////////////////////

    stringstream cp_champion_organism, cp_champion_movement, cp_champion_motorVelocity;
    
    cp_champion_organism << "cp NEAT_organisms/Champion_G" << finalChampionGeneration << "P" << finalChampionPopulation << ".txt ./NEAT_organisms/Champion.txt";
    cp_champion_movement << "cp simulation_files/movement/Champion_G" << finalChampionGeneration << "P" << finalChampionPopulation << ".txt ./simulation_files/movement/Champion.txt";
    cp_champion_motorVelocity << "cp simulation_files/motorVelocity/Champion_G" << finalChampionGeneration << "P" << finalChampionPopulation << ".txt ./simulation_files/motorVelocity/Champion.txt";
        
    if(system((char*)cp_champion_organism.str().c_str()) == -1)
    {
        cerr << "TRAIN ERROR:\tFailed to copy the Champion Organism File" << endl;
    }

    if(system((char*)cp_champion_movement.str().c_str()) == -1)
    {
        cerr << "TRAIN ERROR:\tFailed to copy the Champion Movement File" << endl;
    }

    if(system((char*)cp_champion_motorVelocity.str().c_str()) == -1)
    {
        cerr << "TRAIN ERROR:\tFailed to copy the Champion Motor Velocity File" << endl;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////

    clog << "Fitness champion: " << finalChampionFitness << "\n\n"<< endl;

    delete(vrep);
    delete(simfile);
    delete(fitness);
    
    return(0);
}
