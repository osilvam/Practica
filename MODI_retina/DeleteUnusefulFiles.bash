mkdir -p ./bin/tmp_videos
mkdir -p ./bin/tmp_sf/motorVelocity
mkdir -p ./bin/tmp_sf/movement

for f in ./bin/NEAT_organisms/*; do

	FNAME=${f##*_}
	NAME=${FNAME%.*}

	if [ -f ./bin/videoRecorder/$NAME.avi ]
		then
 		cp ./bin/videoRecorder/$NAME.avi ./bin/tmp_videos
 	fi

 	if [ -f ./bin/simulation_files/motorVelocity/motorVelocity_$NAME.txt ]
		then
 		cp ./bin/simulation_files/motorVelocity/motorVelocity_$NAME.txt ./bin/tmp_sf/motorVelocity
 	fi

 	if [ -f ./bin/simulation_files/movement/movement_$NAME.txt ]
		then
 		cp ./bin/simulation_files/movement/movement_$NAME.txt ./bin/tmp_sf/movement
 	fi

done

rm -f -R ./bin/videoRecorder
rm -f -R ./bin/simulation_files
mv ./bin/tmp_videos ./bin/videoRecorder
mv ./bin/tmp_sf ./bin/simulation_files