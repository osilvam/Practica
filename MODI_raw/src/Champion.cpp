#include "RIAR.hpp"

/* V-REP implementation of an Autonomous Robot that avoid obstacles and advances without
preferred direction. The algotithm uses NEAT to evolve. */

int main(int argc, char* argv[])
{
    srand (time(0));        
    
    if(argc < 3)
    {
        cerr << "ERROR: The number of arguments is incorrect" << endl << "Enter:\targ_1 = genetic_encoding_file\t arg_3 = port_number";
        return -1;
    }

    RobotVREP * vrep = new RobotVREP(false, atoi(argv[2]));

    // ============= VREP INITIALIZATIONS ============= //
    
    Joint * rightWheel = new Joint((char*)"SCALE", (char*)"motor_der");
    Joint * leftWheel = new Joint((char*)"SCALE", (char*)"motor_izq");
    vrep->addJoint(rightWheel);
    vrep->addJoint(leftWheel);

    VisionSensor * visionSensor = new VisionSensor((char*)"VisionSensor");
    vrep->addVisionSensor(visionSensor);

    Object * Modi = new Object((char*)"MODI");
    vrep->addObject(Modi);

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

    Genetic_Encoding champion;
    champion.load(argv[1]);

    // ================================================ //
        
    int sim_time = 0;
    double rightVel = 0.0;
    double leftVel = 0.0;

    vrep->setJointTargetVelocity(rightWheel, 0.0);
    vrep->setJointTargetVelocity(leftWheel, 0.0);

    double rand1 = rand()%201 - 100;
    double rand2 = rand()%201 - 100;
    double rand3 = rand()%201 - 100;

    vector < double > position, orientation;

    position.push_back(rand1/100*.30);
    position.push_back(rand2/100*.30);
    position.push_back(0.02672);

    orientation.push_back(0);
    orientation.push_back(0);
    orientation.push_back(rand3/100*M_PI);
    
    vrep->setObjectPosition(Modi, position);
    vrep->setObjectOrientation(Modi, orientation);

    unsigned char * image;
    Mat frameRGB = Mat(NX, NY, CV_8UC3);
    Mat frameGRAY = Mat(NX, NY, CV_8UC1);          

    vrep->startSimulation(simx_opmode_oneshot_wait);        

    while(sim_time < TIME_SIMULATION)
    {                
        image = vrep->getVisionSensorImage(visionSensor);  
        frameRGB.data = image;
        flip(frameRGB, frameRGB, 0);
        cvtColor(frameRGB,frameGRAY,CV_BGR2GRAY);
        
        Mat tmp = frameGRAY;
        Mat frame = tmp;

        resize(tmp, frame, Size(0,0) , 6.0, 6.0, (int)INTER_NEAREST );

        for(int i = 0; i < NY; i++)
        {
            for(int j = 0;j < NX; j++)
            {
                input.at(i*NX + j) = (double)frame.at<uchar>(i,j)/255*2-1;
            }
        }
        
        input.at(NX*NY) = (double)rightVel/8;
        input.at(NX*NY + 1) = (double)leftVel/8;

        output = champion.eval(input);

        rightVel = output.at(0) + rightVel;
        leftVel = output.at(1) + leftVel;

        if(rightVel > 8) rightVel = 8;
        else if(rightVel < -8) rightVel = -8;
        if(leftVel > 8) leftVel = 8;
        else if(leftVel < -8) leftVel = -8;

        vrep->setJointTargetVelocity(rightWheel,-rightVel);
        vrep->setJointTargetVelocity(leftWheel,leftVel);

        usleep(DELTA_TIME);
        sim_time += DELTA_TIME;
    }            

    vrep->stopSimulation(simx_opmode_oneshot_wait);

    delete(vrep);
    
    return(0);
}
