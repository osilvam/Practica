echo Escriba el nombre del experimento
echo Opciones:
echo -e ' \t ' MODI_raw
echo -e ' \t ' MODI_raw_F2
echo -e ' \t ' MODI_retina
echo -e ' \t ' MODI_retina_F2
read EXPERIMENT
echo Seleccione la escena a usar '('1 ó 2')':
read SCENE

PORT=19996

if [ -d V-REP* ]
	then	
	check_port=$(netstat -an | grep $PORT) 
	until [ ${#check_port} == 0 ]
	do
		PORT=$(( PORT + 1 ))
		check_port=$(netstat -an | grep $PORT) 
	done

	TTTN="../Scene"$SCENE"_FINAL.ttt"

	cd V-REP*
	chmod 755 MultiVrep.sh; bash ./MultiVrep.sh $PORT 1 $TTTN &>/dev/null &
	cd ..
	sleep 5	
	vrep=$(pgrep -n vrep)
else
	echo ERROR: No existe la carpeta que contiene al simulador V-REP
	echo V-REP puede ser descargado desde 'http://www.coppeliarobotics.com/downloads.html'
	exit
fi

echo Escriba T para realizar un entrenamiento o S para mostrar el campeon:
read TAREA

if [ $TAREA == T ]
	then	
	bash Run.bash $EXPERIMENT $TAREA $PORT
elif [ $TAREA == S ]
	then
	cd $EXPERIMENT
	if [ -f "./bin/NEAT_organisms/Champion.txt" ]
		then
		cd ..
		bash Run.bash $EXPERIMENT $TAREA $PORT
	else
		echo ERROR: No existe el archivo Champion.txt
		cd ..
		exit
	fi
else
	echo ERROR: La instrucción no es correcta.
fi

sleep 2

kill -9 $vrep
