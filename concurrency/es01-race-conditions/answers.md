# 1

>  Let’s examine a simple program, “loop.s”. First, just read and un-
derstand it. Then, run it with these arguments (./x86.py -p loop.s
-t 1 -i 100 -R dx) This specifies a single thread, an interrupt
every 100 instructions, and tracing of register %dx. What will %dx
be during the run? Use the -c flag to check your answers; the an-
swers, on the left, show the value of the register (or memory value)
after the instruction on the right has run.


## Answer

Will always be -1

# 2

>  Same code, different flags: (./x86.py -p loop.s -t 2 -i 100 -a
dx=3,dx=3 -R dx) This specifies two threads, and initializes each
%dx to 3. What values will %dx see? Run with -c to check. Does
the presence of multiple threads affect your calculations? Is there a
race in this code?

## Answer

```
Thread 0 dx			Thread 1 dx
	2					3
	1					3
	0					3
	-1					3 -> 2
	-1					1
	-1					0
	-1					-1
			STOP
```

# 3

>  Run this: ./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3
-R dx This makes the interrupt interval small/random; use dif-
ferent seeds (-s) to see different interleavings. Does the interrupt
frequency change anything?

## Answer

Yes it does, in this way we executes only 3 instruction in every thread.
Every three instructions `dx` is decremented.

# 4

> Now, a different program, looping-race-nolock.s, which ac-
cesses a shared variable located at address 2000; we’ll call this vari-
able value. Run it with a single thread to confirm your under-
standing: ./x86.py -p looping-race-nolock.s -t 1 -M 2000 
What is value (i.e., at memory address 2000) throughout the run?
Use -c to check.

## Answer

If in `[2000]` there's 0, then the value will be 1.

# 5

>  Run with multiple iterations/threads: ./x86.py -p
looping-race-nolock.s -t 2 -a bx=3 -M 2000 Why does
each thread loop three times? What is final value of value?

## Answer

The final value will be 6.

We executes three times the `inc` in the Thread 0, and then we
executes three times the `inc` in the Thread 1.

# 6

> Run with random interrupt intervals: ./x86.py -p
looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0 with
different seeds (-s 1, -s 2, etc.) Can you tell by looking at the
thread interleaving what the final value of value will be? Does the
timing of the interrupt matter? Where can it safely occur? Where
not? In other words, where is the critical section exactly?

## Answer

```
 2000          Thread 0                Thread 1
    ?
    ?   1000 mov 2000, %ax
    ?   1001 add $1, %ax
    ?   1002 mov %ax, 2000
    ?   1003 sub  $1, %bx
    ?   ------ Interrupt ------  ------ Interrupt ------
    ?                            1000 mov 2000, %ax
    ?                            1001 add $1, %ax
    ?                            1002 mov %ax, 2000
    ?                            1003 sub  $1, %bx
    ?   ------ Interrupt ------  ------ Interrupt ------
    ?   1004 test $0, %bx
    ?   1005 jgt .top
    ?   ------ Interrupt ------  ------ Interrupt ------
    ?                            1004 test $0, %bx
    ?                            1005 jgt .top
    ?   ------ Interrupt ------  ------ Interrupt ------
    ?   1006 halt
    ?   ----- Halt;Switch -----  ----- Halt;Switch -----
    ?                            1006 halt
```

Will be 2.
The timing interrupt in this case doesn't mutter, if the timing would
be set before the instruction `mov %ax, 2000`, then race condition eheh
:).

# 7

> Now examine fixed interrupt intervals: ./x86.py -p
looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1 What
will the final value of the shared variable value be? What about
when you change -i 2, -i 3, etc.? For which interrupt intervals
does the program give the “correct” answer?

## Answer

```
-i 1 --> [2000] = 1 [Race condition]
-i 2 --> [2000] = 1 [Race condition]
-i 3 --> [2000] = 2 [Good yeay]
-i 3 + x --> [2000] = 2 [Good yeay]
```

