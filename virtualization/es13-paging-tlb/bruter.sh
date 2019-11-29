#!/bin/zsh

NTRIALS=2000

make
if [[ -e log.csv ]] 
then 
	rm log.csv
fi
echo "NPAGES,TIME" > log.csv
i=2
while [[ $i != 65536 ]]
do
	./tlb.exe ${i} ${NTRIALS} >> log.csv
	let "i = $i * 2"
done
./plotter.py

