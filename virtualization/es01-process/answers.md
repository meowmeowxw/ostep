# 1

> Run process-run.py with the following flags: -l 5:100,5:100.
What should the CPU utilization be (e.g., the percent of time the
CPU is in use?) Why do you know this? Use the -c and -p flags to
see if you were right.

## Answer

```
./process-run.py -l 5:100,5:100 -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2     RUN:cpu      READY          1            
  3     RUN:cpu      READY          1            
  4     RUN:cpu      READY          1            
  5     RUN:cpu      READY          1            
  6        DONE    RUN:cpu          1            
  7        DONE    RUN:cpu          1            
  8        DONE    RUN:cpu          1            
  9        DONE    RUN:cpu          1            
 10        DONE    RUN:cpu          1            

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

The cpu is always busy because the processes don't requests any IO use.

# 2

> Now run with these flags: ./process-run.py -l 4:100,1:0.
These flags specify one process with 4 instructions (all to use the
CPU), and one that simply issues an I/O and waits for it to be done.
How long does it take to complete both processes? Use -c and -p
to find out if you were right.

## Answer

In the figure 4.4 of the book, we can see that the cpu take 4 `tick` to
perform the IO.

To deduce how long it takes to complete the two processes we can compute:

```
time = 4 instruction P0 + 1 request to IO (P1) + 4 IO done (P2) +
1 final instruction = 10
```

```
./process-run.py -l 4:100,1:0 -c -p     
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2     RUN:cpu      READY          1            
  3     RUN:cpu      READY          1            
  4     RUN:cpu      READY          1            
  5        DONE     RUN:io          1            
  6        DONE    WAITING                     1 
  7        DONE    WAITING                     1 
  8        DONE    WAITING                     1 
  9        DONE    WAITING                     1 
 10*       DONE       DONE                       

Stats: Total Time 10
Stats: CPU Busy 5 (50.00%)
Stats: IO Busy  4 (40.00%)
```

# 3 

> Switch the order of the processes: -l 1:0,4:100. What happens
now? Does switching the order matter? Why? (As always, use -c
and -p to see if you were right)

## Answer

If we switch the order, then it will take less time:

```
P0 runs request IO.
P1 runs 4 instructions and at the same time P0 waits.
P0 done (IO Finish) and at the same time P1 done.

= 6
```

```
./process-run.py -l 1:0,4:100 -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6*       DONE       DONE
```

# 4

> We’ll now explore some of the other flags. One important flag is
-S, which determines how the system reacts when a process is-
sues an I/O. With the flag set to SWITCH ON END, the system
will NOT switch to another process while one is doing I/O, in-
stead waiting until the process is completely finished. What hap-
pens when you run the following two processes (-l 1:0,4:100
-c -S SWITCH ON END), one doing I/O and the other doing CPU
work?


```
P0 run request IO
P0 waits 4 instruction
P0 done and P1 run 4 instruction
```

```
./process-run.py -l 1:0,4:100 -S SWITCH_ON_END -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING      READY                     1 
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6*       DONE    RUN:cpu          1            
  7        DONE    RUN:cpu          1            
  8        DONE    RUN:cpu          1            
  9        DONE    RUN:cpu          1            

Stats: Total Time 9
Stats: CPU Busy 5 (55.56%)
Stats: IO Busy  4 (44.44%)
```

# 5

>  Now, run the same processes, but with the switching behavior set
to switch to another process whenever one is WAITING for I/O (-l
1:0,4:100 -c -S SWITCH ON IO). What happens now? Use -c
and -p to confirm that you are right.

## Answer

Same as `#3`

# 6

> One other important behavior is what to do when an I/O com-
pletes. With -I IO RUN LATER, when an I/O completes, the pro-
cess that issued it is not necessarily run right away; rather, whatever
was running at the time keeps running. What happens when you
run this combination of processes? (Run ./process-run.py -l
3:0,5:100,5:100,5:100 -S SWITCH ON IO -I IO RUN LATER
-c -p) Are system resources being effectively utilized?

## Answer

We have `P[1-3]`, which executes 5 instructions and `P0` which performs
3 IO.

Compute the time:

```
P0 requests IO
P1 runs 5 instructions while P0 waits
P2 runs 5 instructions while P0 is ready
P3 runs 5 instructions while P0 is ready
P0 requests another IO --> 5 instructions (request + wait)
P0 requests another IO --> 5 instruction (request + wait)
P0 done.

= 27
```

# 7 

> Now run the same processes, but with -I IO RUN IMMEDIATE set,
which immediately runs the process that issued the I/O. How does
this behavior differ? Why might running a process that just com-
pleted an I/O again be a good idea?
 
## Answer

```
P0 requests IO
P1 runs 4 instructions, and P0 waits.
P0 requests IO
P1 finish its last instruction and P2 runs 3 instructions
P0 requests IO
P2 runs 2 instruction and P3 runs 2 instructions.
P0 Done and P3 runs its last 3 instructions.

= 1 + 4 + 1 + 4 + 1 + 4 + 3 = 18
```

# 8

>  Now run with some randomly generated processes: -s 1 -l 3:50,3:50
or -s 2 -l 3:50,3:50 or -s 3 -l 3:50,3:50. See if you can
predict how the trace will turn out. What happens when you use
the flag -I IO RUN IMMEDIATE vs. -I IO RUN LATER? What hap-
pens when you use -S SWITCH ON IO vs. -S SWITCH ON END?

## Answer

`SWITCH_ON_IO` is always faster the `SWITCH_ON_END`.

`IO_RUN_IMMEDIATE` performs equal to `IO_RUN_LATER`.

