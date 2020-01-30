#!/bin/bash

if [[ -e selected.txt ]]
	then rm selected.txt
fi

FILES=$(/bin/ls /usr/include/*.h)
for i in ${FILES}
do
	LINES=$(/bin/cat ${i} | /bin/grep "ifdef" | wc -l)
	if (( "${LINES}" >= "10" ))
	then
		SELECTED=$(/bin/cat ${i} | /bin/grep "ifdef" | head -n 5)
		echo -e "${SELECTED} ${LINES} ${i}\n"
		echo ${SELECTED} >> selected.txt
	fi
done

sort selected.txt > final.txt

