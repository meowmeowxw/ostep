#!/bin/sh

make
./counter.exe
./counter-lock.exe
./counter-no-lock.exe
./counter-lock-optimized.exe
# Timing on Intel i7-8565U (8)
# > ./counter.exe 
# [*] expected value: 800000000
# [*] real value: 800000000
# [*] time: 1409696957 ns
# 
# > ./counter-lock.exe 
# [*] expected value: 800000000
# [*] real value: 800000000
# [*] time: 85068322987 ns
# 
# > ./counter-no-lock.exe 
# [*] expected value: 800000000
# [*] real value: 114382298
# [*] time: 3370050849 ns
# 
# > ./counter-lock-optimized.exe 
# [*] expected value: 800000000
# [*] real value: 800000000
# [*] time: 1186460699 ns

