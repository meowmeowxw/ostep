#!/bin/bash

ARG=$(/bin/ls -S1 /home/meowmeow/Projects/cryptography/cryptopals/*.py | tail -n 7)
./es03-counter.sh $ARG >&2

