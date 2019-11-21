# 1

>  Compute the response time and turnaround time when running
three jobs of length 200 with the SJF and FIFO schedulers.

## Answer

```
T_turnaround = T_completion - T_arrival
T_response = T_firstrun - T_arrival
```

With SJF:

```
T_turnaround = (200 + 400 + 600) / 3 = 400
T_response = (0 + 200 + 400) / 3 = 200
```

With FIFO:

```
T_turnaround = (200 + 400 + 600) / 3 = 400
T_response = (0 + 200 + 400) / 3 = 200
```

# 2

> Now do the same but with jobs of different lengths: 100, 200, and
300.

## Answer

With SJF:

```
T_turnaround = (100 + (200 + 100) + (300 + (200 + 100))) / 3 = 333.33
T_response = (0 + 100 + (100 + 200)) / 3 = 133.33
```

With FIFO:

```
T_turnaround = 333.33
T_response = 133.33
```

# 3

>  Now do the same, but also with the RR scheduler and a time-slice
of 1

## Answer

```
J0 will finish at (1 + 1 + 1) * 100 - 2 = 298
J1 will finish at (300 + (1 + 1) * 100 - 1) = 499
J3 will finish at (500 + (1) * 100) = 600
T_turnaround = (600 + 499 + 298) / 3 = 456.66
T_response = (0 + 1 + 2) / 3 = 1
```

# 4

> For what types of workloads does SJF deliver the same turnaround
times as FIFO?

## Answer

When the jobs have all the same length.

# 5

>  For what types of workloads and quantum lengths does SJF deliver
the same response times as RR?

## Answer

They have the same response times if RR runs only one job or when the time 
slice is equal to the jobs length.

# 6

> What happens to response time with SJF as job lengths increase?
Can you use the simulator to demonstrate the trend?

zsh script:

```sh
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
```

Output:

```
Length job : 100	Response time : 100.00
Length job : 200	Response time : 200.00
Length job : 300	Response time : 300.00
Length job : 400	Response time : 400.00
Length job : 500	Response time : 500.00
Length job : 600	Response time : 600.00
Length job : 700	Response time : 700.00
Length job : 800	Response time : 800.00
Length job : 900	Response time : 900.00
Length job : 1000	Response time : 1000.00
Length job : 1100	Response time : 1100.00
Length job : 1200	Response time : 1200.00
Length job : 1300	Response time : 1300.00
Length job : 1400	Response time : 1400.00
Length job : 1500	Response time : 1500.00
```

# 7

>  What happens to response time with RR as quantum lengths in-
crease? Can you write an equation that gives the worst-case re-
sponse time, given N jobs?


## Answer

We can use Gauss formula to sum the first `n` natural numbers.

In this case if we have that a job take as long as `q`, where `q = time slice`,
then the worst case is `((q-1) * q) / 2`.
