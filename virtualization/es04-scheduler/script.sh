#!/bin/zsh

START=100
END=1500
PASS=100

for i in {${START}..${END}..${PASS}}
do
	echo -n "Length job : $i"
	TURN=$(./scheduler.py -j 3 -p SJF -l ${i},${i},${i} -c | \
		tail -n 2 | grep Response | awk '{print $4}')
	echo "\tResponse time : $TURN"
done
