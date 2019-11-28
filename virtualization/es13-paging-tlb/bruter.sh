#!/bin/zsh

NTRIALS=1000
if [[ -e log.csv ]]; then rm log.csv; fi
for i in {0..1024..4}
do
	./tlb $i $NTRIALS 1>&2 2> log.csv 
done

