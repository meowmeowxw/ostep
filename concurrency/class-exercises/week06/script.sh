#!/bin/bash

FILES=""
while IFS= read -r LINE || [[ -n "$LINE" ]]; do
	FILES="${FILES} ${LINE}"
done
i=0
newest=""
for f in ${FILES}
do
	if [[ $f -nt $newest ]]
	then
		newest=$f
	fi
	let "i = ${i} + 1"
done
echo $newest

