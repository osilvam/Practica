#Se debe ejecutar con parametro 1: numero de puerto inicial; parametro 2: cantidad de simuladores; parametro 3: ruta al .ttt
number=$1
for (( i = 0; i < $2; i++ )); do
	rm -f remoteApiConnections.txt
	echo "portIndex1_port                 = $number
portIndex1_debug                = false
portIndex1_syncSimTrigger       = true" >> remoteApiConnections.txt
	./vrep.sh $3 &
	((number = number + 1))
	sleep 5
done
