EXECUTABLE1 = RIAR
EXECUTABLE2 = Champion
EXECUTABLE3 = RetinaFilter
VPATH = ./src ./headers
CC = g++ -O3 `pkg-config --cflags --libs opencv` -std=c++0x
CFLAGS = -g -Wall -I./src -I./headers  -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 
LDFLAGS = -lpthread -L. -lhyperneat -lrobotlib -lneat `pkg-config --libs opencv`
DATE = `date +'%F--%T'` 
CURRENTFOLDER = $(notdir $(shell pwd))
SPATH = ./saveTrains_$(CURRENTFOLDER)/$(DATE)
CNAME = ../saveTrains.zip

all: CalcFunctions.o Fitness.o SimFiles.o Retina.o
	@echo "Linking all objects to ./bin/$(EXECUTABLE1)"
	@$(CC) $(CFLAGS) ./src/RIAR.cpp ./objects/Retina.o ./objects/Fitness.o ./objects/SimFiles.o ./objects/CalcFunctions.o -o ./bin/$(EXECUTABLE1) $(LDFLAGS)
	@echo "Linking all objects to ./bin/$(EXECUTABLE2)"
	@$(CC) $(CFLAGS) ./src/Champion.cpp ./objects/Retina.o ./objects/Fitness.o ./objects/SimFiles.o ./objects/CalcFunctions.o -o ./bin/$(EXECUTABLE2) $(LDFLAGS)
	@echo "Linking all objects to ./bin/$(EXECUTABLE3)"
	@$(CC) $(CFLAGS) ./src/RetinaFilter.cpp ./objects/Retina.o -o ./bin/$(EXECUTABLE3) $(LDFLAGS)

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
	@rm -f -R ./bin/simulation_files*
	@rm -f ./bin/*.txt
	@if [ -d saveTrains* ] ; \
	then \
	    zip -r $(CNAME) ./saveTrains* ; \
		rm -f -R ./saveTrains* ; \
	fi;	

runRIAR:
	@cd ./bin; ./$(EXECUTABLE1) user_def GeneticEncoding1024

runCHAMPION:
	@cd ./bin; ./$(EXECUTABLE2) ./NEAT_organisms/Champion.txt

save:
	@mkdir -p $(SPATH); cp -R ./bin/* $(SPATH);


