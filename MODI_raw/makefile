EXECUTABLE1 = RIAR
EXECUTABLE2 = Champion
EXECUTABLE3 = RetinaFilter
VPATH = ./src ./headers
CC = g++ -O3 `pkg-config --cflags --libs opencv` 
CFLAGS = -g -Wall -I./src -I./headers  -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 
LDFLAGS = -lpthread -L. -lhyperneat -lrobotlib -lneat `pkg-config --libs opencv`
DATE = `date +'%F--%T'`
SPATH = ./saveTrains/$(DATE)
CNAME = saveTrains.zip

all: RIAR.o Champion.o RetinaFilter.o CalcFunctions.o Fitness.o SimFiles.o Retina.o
	@mkdir -p bin/NEAT_organisms
	@rm -f ./bin/NEAT_organisms/*
	@echo "Linking all objects to ./bin/RIAR"
	@$(CC) $(CFLAGS) ./objects/Fitness.o ./objects/Retina.o ./objects/SimFiles.o ./objects/CalcFunctions.o ./objects/RIAR.o  -o ./bin/$(EXECUTABLE1) $(LDFLAGS)
	@echo "Linking all objects to ./bin/Champion"
	@$(CC) $(CFLAGS) ./objects/Fitness.o ./objects/Retina.o ./objects/SimFiles.o ./objects/CalcFunctions.o ./objects/Champion.o  -o ./bin/$(EXECUTABLE2) $(LDFLAGS)
	@echo "Linking all objects to ./bin/RetinaFilter"
	@$(CC) $(CFLAGS) ./objects/Fitness.o ./objects/Retina.o ./objects/SimFiles.o ./objects/CalcFunctions.o ./objects/RetinaFilter.o  -o ./bin/$(EXECUTABLE3) $(LDFLAGS)
	

RIAR.o: RIAR.cpp 
	@mkdir -p objects
	@echo "Compiling RIAR.cpp to RIAR.o"
	@$(CC) $(CFLAGS) -c ./src/RIAR.cpp -o ./objects/RIAR.o

Champion.o: Champion.cpp 
	@mkdir -p objects
	@echo "Compiling Champion.cpp to Champion.o"
	@$(CC) $(CFLAGS) -c ./src/Champion.cpp -o ./objects/Champion.o

RetinaFilter.o: RetinaFilter.cpp 
	@mkdir -p objects
	@echo "Compiling RetinaFilter.cpp to RetinaFilter.o"
	@$(CC) $(CFLAGS) -c ./src/RetinaFilter.cpp -o ./objects/RetinaFilter.o

CalcFunctions.o: CalcFunctions.cpp 
	@mkdir -p objects
	@echo "Compiling CalcFunctions.cpp to CalcFunctions.o"
	@$(CC) $(CFLAGS) -c ./src/CalcFunctions.cpp -o ./objects/CalcFunctions.o

Fitness.o: Fitness.cpp
	@mkdir -p objects
	@echo "Compiling Fitness.cpp to Fitness.o"
	@$(CC) $(CFLAGS) -c ./src/Fitness.cpp -o ./objects/Fitness.o

SimFiles.o: SimFiles.cpp
	@mkdir -p objects
	@echo "Compiling SimFiles.cpp to SimFiles.o"
	@$(CC) $(CFLAGS) -c ./src/SimFiles.cpp -o ./objects/SimFiles.o

Retina.o: Retina.cpp
	@mkdir -p objects
	@echo "Compiling Retina.cpp to Retina.o"
	@$(CC) $(CFLAGS) -c ./src/Retina.cpp -o ./objects/Retina.o


clean:
	@rm -f -R ./objects
	@rm -f ./bin/$(EXECUTABLE1)
	@rm -f ./bin/$(EXECUTABLE2)
	@rm -f ./bin/$(EXECUTABLE3)
	@rm -f -R ./bin/NEAT_organisms
	@rm -f -R ./bin/videoRecorder*
	@rm -f ./bin/*.txt
	@zip -r $(CNAME) ./saveTrains/*
	@rm -f -R ./saveTrains

run:
	@rm -f ./bin/NEAT_organisms/*
	@rm -f ./bin/*.txt
	@cd ./bin; ./RIAR user_def GeneticEncoding1024

save:
	@mkdir -p $(SPATH); cp -R ./bin/* $(SPATH);


