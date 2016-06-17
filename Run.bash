cd $1; cd bin
if [ $2 == T ]
	then
	./RIAR user_def GeneticEncoding1024 $3
elif [ $2 == S ]
	then
	./Champion ./NEAT_organisms/Champion.txt $3
fi
cd ../..