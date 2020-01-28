#!/bin/bash

FILE="./fall.txt"
TMP="./tmp.txt"

if [[ -e ${TMP} ]]
then 
    rm ${TMP}
fi
while read DATE PLACE REASON DAMAGE
do
    NUM=$(grep ${REASON} ${FILE}| wc -l)
    echo "${REASON} ${NUM}" >> ${TMP}
done < ${FILE}
sort ${TMP} | uniq

