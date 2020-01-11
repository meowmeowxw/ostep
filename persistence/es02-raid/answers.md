# 1

>  Use the simulator to perform some basic RAID mapping tests. Run
with different levels (0, 1, 4, 5) and see if you can figure out the
mappings of a set of requests. For RAID-5, see if you can figure out
the difference between left-symmetric and left-asymmetric layouts.
Use some different random seeds to generate different problems
than above.

## Answer

```
./raid.py -L 0 -n 3 -R 20   
ARG blockSize 4096
ARG seed 0
ARG numDisks 4
ARG chunkSize 4k
ARG numRequests 3
ARG reqSize 4k
ARG workload rand
ARG writeFrac 0
ARG randRange 20
ARG level 0
ARG raid5 LS
ARG reverse False
ARG timing False

LOGICAL READ from addr:16 size:4096
  Physical reads/writes? -> [Disk 16 % 4, Offset 16 / 4]

LOGICAL READ from addr:8 size:4096
  Physical reads/writes? -> [Disk 8 % 4, Offset 8 / 4]

LOGICAL READ from addr:10 size:4096
  Physical reads/writes? -> [Disk 10 % 4, Offset 10/4]
```

```
./raid.py -L 1 -n 3 -R 20   
ARG blockSize 4096
ARG seed 0
ARG numDisks 4
ARG chunkSize 4k
ARG numRequests 3
ARG reqSize 4k
ARG workload rand
ARG writeFrac 0
ARG randRange 20
ARG level 1
ARG raid5 LS
ARG reverse False
ARG timing False

LOGICAL READ from addr:16 size:4096
  Physical reads/writes? [Disk 16 % 2, Offset 16 / 2]

LOGICAL READ from addr:8 size:4096
  Physical reads/writes? [Disk 8 % 2, Offset 8 / 2]

LOGICAL READ from addr:10 size:4096
  Physical reads/writes? [Disk 10 % 2, Offset 10 / 2]
```
