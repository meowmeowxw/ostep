#!/bin/bash

LINES=$(ls -1 /usr/include/linux/*f*.h)
for l in ${LINES}
do 
	LENGTH=$(cat $l | wc -l)
	if (( $LENGTH >= 10 && $LENGTH <= 100 ))
	then
		LAST=$(cat $l | tail -n 1 | wc -c)
		echo -e "$l\t\t\t\t$LENGTH\t$LAST"
	fi
done

