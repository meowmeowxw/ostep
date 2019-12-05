# 1

> First build main-race.c. Examine the code so you can see the (hopefully
obvious) data race in the code. Now run helgrind (by typing valgrind
--tool=helgrind main-race) to see how it reports the race. Does it
point to the right lines of code? What other information does it give to you?

## Answer

Yes, it reports me the right line `(9)`.

# 2

> What happens when you remove one of the offending lines of code? Now
add a lock around one of the updates to the shared variable, and then around
both. What does helgrind report in each of these cases?

## Answer

In this case no report from helgrind.

# 3

> Now let’s look at main-deadlock.c. Examine the code. This code has a
problem known as deadlock (which we discuss in much more depth in a
forthcoming chapter). Can you see what problem it might have?

## Answer

We start two thread, one with arg `0` and the other with arg `1`.

If `Thread 0` is executed first, then it can lock `m1`, then when a 
context switch occurs the `Thread 1` will lock `m2`.
Another context switch and `Thread 0` will be blocked in line `12` 
`Pthread_mutex_lock(&m2);`, another context switch and `Thread 1` will
be blocked in line `15` `Pthread_mutex_lock(&m1);`, because `Thread 0` has
`m1` blocked.

This is deadlock, `Thread 0` nor `Thread 1` can continue their executions.

# 4

> Now run helgrind on this code. What does helgrind report?

## Answer

```
> valgrind --tool=helgrind ./main-deadlock
==2722== Helgrind, a thread error detector
==2722== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==2722== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==2722== Command: ./main-deadlock
==2722==
==2722== ---Thread-Announcement------------------------------------------
==2722==
==2722== Thread #3 was created
==2722==    at 0x49A52C2: clone (in /usr/lib/libc-2.30.so)
==2722==    by 0x488C38B: create_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x488DCFB: pthread_create@@GLIBC_2.2.5 (in /usr/lib/libpthread-2.30
.so)
==2722==    by 0x483F677: pthread_create_WRK (hg_intercepts.c:427)
==2722==    by 0x109443: Pthread_create (mythreads.h:50)
==2722==    by 0x109578: main (main-deadlock.c:25)
==2722==
==2722== ----------------------------------------------------------------
==2722==
==2722== Thread #3: lock order "0x10C0A0 before 0x10C0E0" violated
==2722==
==2722== Observed (incorrect) order is: acquisition of lock at 0x10C0E0
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x1094F5: worker (main-deadlock.c:14)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==
==2722==  followed by a later acquisition of lock at 0x10C0A0
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x109501: worker (main-deadlock.c:15)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==
==2722== Required order was established by acquisition of lock at 0x10C0A0
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x1094DB: worker (main-deadlock.c:11)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==
==2722==  followed by a later acquisition of lock at 0x10C0E0
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x1094E7: worker (main-deadlock.c:12)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==
==2722==  Lock at 0x10C0A0 was first observed
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x1094DB: worker (main-deadlock.c:11)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==  Address 0x10c0a0 is 0 bytes inside data symbol "m1"
==2722==
==2722==  Lock at 0x10C0E0 was first observed
==2722==    at 0x483CC3F: mutex_lock_WRK (hg_intercepts.c:909)
==2722==    by 0x1092C6: Pthread_mutex_lock (mythreads.h:22)
==2722==    by 0x1094E7: worker (main-deadlock.c:12)
==2722==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==2722==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==2722==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==2722==  Address 0x10c0e0 is 0 bytes inside data symbol "m2"
==2722==
==2722==
==2722==
==2722== Use --history-level=approx or =none to gain increased speed, at
==2722== the cost of reduced accuracy of conflicting-access information
==2722== For lists of detected and suppressed errors, rerun with: -s
==2722== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 7 from 7)
```

Basically lock order violated --> `deadlock` (?)

# 5

> Now run helgrind on main-deadlock-global.c. Examine the code;
does it have the same problem that main-deadlock.c has? Should helgrind
be reporting the same error? What does this tell you about tools like helgrind?

## Answer

> It finds a deadlock even here, but I'm pretty sure this is a false positive.

# 6

> Let’s next look at main-signal.c. This code uses a variable (done) to
signal that the child is done and that the parent can now continue. Why is
this code inefficient? (what does the parent end up spending its time doing,
particularly if the child thread takes a long time to complete?)

## Answer

The parent iterate `;` instructions in the `while (done == 0)`, so it's not 
doing anything.

# 7

> Now run helgrind on this program. What does it report? Is the code
correct?

## Answer

```
> valgrind --tool=helgrind ./main-signal
==7122== Helgrind, a thread error detector
==7122== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==7122== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==7122== Command: ./main-signal
==7122==
this should print first
==7122== ---Thread-Announcement------------------------------------------
==7122==
==7122== Thread #1 is the program's root thread
==7122==
==7122== ---Thread-Announcement------------------------------------------
==7122==
==7122== Thread #2 was created
==7122==    at 0x49A52C2: clone (in /usr/lib/libc-2.30.so)
==7122==    by 0x488C38B: create_thread (in /usr/lib/libpthread-2.30.so)
==7122==    by 0x488DCFB: pthread_create@@GLIBC_2.2.5 (in /usr/lib/libpthread-2.30
.so)
==7122==    by 0x483F677: pthread_create_WRK (hg_intercepts.c:427)
==7122==    by 0x109453: Pthread_create (mythreads.h:50)
==7122==    by 0x109530: main (main-signal.c:15)
==7122==
==7122== ----------------------------------------------------------------
==7122==
==7122== Possible data race during read of size 4 at 0x10C08C by thread #1
==7122== Locks held: none
==7122==    at 0x109532: main (main-signal.c:16)
==7122==
==7122== This conflicts with a previous write of size 4 by thread #2
==7122== Locks held: none
==7122==    at 0x1094E5: worker (main-signal.c:9)
==7122==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==7122==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==7122==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==7122==  Address 0x10c08c is 0 bytes inside data symbol "done"
==7122==
==7122== ----------------------------------------------------------------
==7122==
==7122== Possible data race during write of size 1 at 0x52711A5 by thread #1
==7122== Locks held: none
==7122==    at 0x4845526: mempcpy (vg_replace_strmem.c:1537)
==7122==    by 0x4927811: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib/libc-2.30.so)
==7122==    by 0x491D157: puts (in /usr/lib/libc-2.30.so)
==7122==    by 0x109547: main (main-signal.c:18)
==7122==  Address 0x52711a5 is 21 bytes inside a block of size 1,024 alloc'd
==7122==    at 0x48397CF: malloc (vg_replace_malloc.c:309)
==7122==    by 0x491AB43: _IO_file_doallocate (in /usr/lib/libc-2.30.so)
==7122==    by 0x49295BF: _IO_doallocbuf (in /usr/lib/libc-2.30.so)
==7122==    by 0x49287A7: _IO_file_overflow@@GLIBC_2.2.5 (in /usr/lib/libc-2.30.so
)
==7122==    by 0x4927895: _IO_file_xsputn@@GLIBC_2.2.5 (in /usr/lib/libc-2.30.so)
==7122==    by 0x491D157: puts (in /usr/lib/libc-2.30.so)
==7122==    by 0x1094E4: worker (main-signal.c:8)
==7122==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==7122==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==7122==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==7122==  Block was alloc'd by thread #2
==7122==
this should print last
==7122==
==7122== Use --history-level=approx or =none to gain increased speed, at
==7122== the cost of reduced accuracy of conflicting-access information
==7122== For lists of detected and suppressed errors, rerun with: -s
==7122== ERROR SUMMARY: 23 errors from 2 contexts (suppressed: 40 from 40)
```

The most important part in this case I guess it is:

```
=7122== Possible data race during read of size 4 at 0x10C08C by thread #1
==7122== Locks held: none
==7122==    at 0x109532: main (main-signal.c:16)
==7122==
==7122== This conflicts with a previous write of size 4 by thread #2
==7122== Locks held: none
==7122==    at 0x1094E5: worker (main-signal.c:9)
==7122==    by 0x483F876: mythread_wrapper (hg_intercepts.c:389)
==7122==    by 0x488D4CE: start_thread (in /usr/lib/libpthread-2.30.so)
==7122==    by 0x49A52D2: clone (in /usr/lib/libc-2.30.so)
==7122==  Address 0x10c08c is 0 bytes inside data symbol "done"
```

If the variable is cached in cpu registers the main does useless instructions,
volatile is recommended in this case.
If mutex, and condition variables are used, then it's better to not set
variables volatile.

[Intel paper](http://web.archive.org/web/20190219170904/https://software.intel.com/en-us/blogs/2007/11/30/volatile-almost-useless-for-multi-threaded-programming/)

[Stackoverflow thread](https://stackoverflow.com/questions/78172/using-c-pthreads-do-shared-variables-need-to-be-volatile)

[Dekker's algorithm fail](http://jakob.engbloms.se/archives/65)

Another problem is that there isn't a `pthread_join`.

# 8

> Now look at a slightly modified version of the code, which is found in
main-signal-cv.c. This version uses a condition variable to do the sig-
naling (and associated lock). Why is this code preferred to the previous
version? Is it correctness, or performance, or both?

## Answer

This is preferred because the signals/conditions are handled by pthreads.
It's less likely to have a bug.

# 9

> Once again run helgrind on main-signal-cv. Does it report any errors?

## Answer

No errors this time.

