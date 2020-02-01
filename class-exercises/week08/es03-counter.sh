#!/bin/bash

EVENFILE="/tmp/even"
ODDFILE="/tmp/odd"
COUNTER=0

if (( $# >= 10 ))
then
    echo "too many arguments"
    exit 1
fi

if [[ -e $EVENFILE ]]
then
    rm $EVENFILE
fi

if [[ -e $ODDFILE ]]
then
    rm $ODDFILE
fi

for i in $*
do
    if (( COUNTER % 2 == 0 ))
    then
        cat $i >> $EVENFILE
    else
        cat $i >> $ODDFILE
    fi
    let "COUNTER = COUNTER + 1"
done

EVEN=$(wc -l ${EVENFILE})
ODD=$(wc -l ${ODDFILE})

echo $EVEN
echo $ODD >&2 2>/dev/null

