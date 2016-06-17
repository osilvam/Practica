if ! [[ -d RobotLib ]]
	then
	echo "Downloading RobotLib library from git repository"
	git clone https://github.com/osilvam/RobotLib.git
	cd RobotLib; make; sudo make install; cd ..;
else
	cd RobotLib
	LOCAL=$(git rev-parse @)
	REMOTE=$(git rev-parse @{u})
	if [ $LOCAL = $REMOTE ]
		then
		echo "RobotLib is already downloaded and Up-to-date"
		cd ..
	else
		cd ..
		rm -f -R /RobotLib
		echo "Downloading RobotLib library from git repository"
		git clone https://github.com/osilvam/RobotLib.git
		cd RobotLib; make; sudo make install; cd ..;
	fi
fi

VREPF=$(ls -d */ | grep V-REP)
cd $VREPF
if ! [[ -f MultiVrep.sh ]]
	then
	cp ../MultiVrep.sh .
fi
cd ..

if ! [[ -d NEAT ]]
	then
	echo "Downloading NEAT library from git repository"
	git clone https://github.com/osilvam/NEAT.git
	cd NEAT; make; sudo make install; cd ..;
else
	cd NEAT
	LOCAL=$(git rev-parse @)
	REMOTE=$(git rev-parse @{u})
	if [ $LOCAL = $REMOTE ]
		then
		echo "NEAT is already downloaded and Up-to-date"		
		cd ..
	else
		cd ..
		rm -f -R /NEAT
		echo "Downloading NEAT library from git repository"
		git clone https://github.com/osilvam/NEAT.git
		cd NEAT; make; sudo make install; cd ..;
	fi
fi

check_opencv_installed=$(pkg-config --modversion opencv)
if [ ${#check_opencv_installed} == 0 ]
	then
	echo "Downloading and Installing opencv library"
	sudo apt-get install build-essential
	sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
	sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
	git clone https://github.com/Itseez/opencv.git
	cd opencv; mkdir release; cd release; cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..; make; sudo make install
	cd lib; export LD_LIBRARY_PATH=$PWD; cd ../../..
else
	echo "Opencv is already installed"
fi

cd MODI_raw; make; cd ..;
cd MODI_raw_F2; make; cd ..;
cd MODI_retina; make; cd ..;
cd MODI_retina_F2; make; cd ..;
