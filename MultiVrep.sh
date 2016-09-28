#Se debe ejecutar con parametro 1: numero de puerto inicial; parametro 2: cantidad de simuladores; parametro 3: ruta al .ttt
number=$1
PIDS=""
for (( i = 0; i < $2; i++ )); do
	rm -f remoteApiConnections.txt
	echo "portIndex1_port                 = $number
portIndex1_debug                = false
portIndex1_syncSimTrigger       = true" >> remoteApiConnections.txt
	./vrep.sh $3 &>/dev/null &
	PID=$!
	printf "$(ps -fe | grep [v]rep | grep $PID | grep -v .sh | awk '{print $2}')"
	PID=$(ps -fe | grep [v]rep | grep $PID | grep -v .sh | awk '{print $2}')
	PIDS="${PIDS}${PID} "
	((number = number + 1))
	sleep 5
done

echo "${PIDS}"