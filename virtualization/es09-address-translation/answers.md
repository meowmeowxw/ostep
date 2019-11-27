# 1 

>  Run with seeds 1, 2, and 3, and compute whether each virtual ad-
dress generated by the process is in or out of bounds. If in bounds,
compute the translation.

## Answer

```
./relocation.py -s 1

ARG seed 1
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x0000363c (decimal 13884)
  Limit  : 290

Virtual Address Trace
  VA  0: 0x0000030e (decimal:  782) --> PA or segmentation violation?
  VA  1: 0x00000105 (decimal:  261) --> PA or segmentation violation?
  VA  2: 0x000001fb (decimal:  507) --> PA or segmentation violation?
  VA  3: 0x000001cc (decimal:  460) --> PA or segmentation violation?
  VA  4: 0x0000029b (decimal:  667) --> PA or segmentation violation?

For each virtual address, either write down the physical address it translates to
OR write down that it is an out-of-bounds address (a segmentation violation). For
this problem, you should assume a simple virtual address space of a given size.
```

```
Limit = 13884 + 290 = 14174 = 0x375e
```

But we don't need to too many calculus, we can just see which `VA` are
higher than 290 = 0x122, so there are all segmentations a part from
`VA 1`.

```
./relocation.py -s 2  

ARG seed 2
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00003ca9 (decimal 15529)
  Limit  : 500

Virtual Address Trace
  VA  0: 0x00000039 (decimal:   57) --> PA or segmentation violation?
  VA  1: 0x00000056 (decimal:   86) --> PA or segmentation violation?
  VA  2: 0x00000357 (decimal:  855) --> PA or segmentation violation?
  VA  3: 0x000002f1 (decimal:  753) --> PA or segmentation violation?
  VA  4: 0x000002ad (decimal:  685) --> PA or segmentation violation?
```

this time the last three are segmeentation violation, while the firsts two
are valids.

```
./relocation.py -s 3   

ARG seed 3
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x000022d4 (decimal 8916)
  Limit  : 316

Virtual Address Trace
  VA  0: 0x0000017a (decimal:  378) --> PA or segmentation violation?
  VA  1: 0x0000026a (decimal:  618) --> PA or segmentation violation?
  VA  2: 0x00000280 (decimal:  640) --> PA or segmentation violation?
  VA  3: 0x00000043 (decimal:   67) --> PA or segmentation violation?
  VA  4: 0x0000000d (decimal:   13) --> PA or segmentation violation?
```

in this case the lasts two are valid --> `< 0x13c = 316`.

# 2

>  Run with these flags: -s 0 -n 10. What value do you have set
-l (the bounds register) to in order to ensure that all the generated
virtual addresses are within bounds?

## Answer

```
./relocation.py -s 0 -n 10

ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x0000360b (decimal 13835)
  Limit  : 276

Virtual Address Trace
  VA  0: 0x00000308 (decimal:  776) --> PA or segmentation violation?
  VA  1: 0x000001ae (decimal:  430) --> PA or segmentation violation?
  VA  2: 0x00000109 (decimal:  265) --> PA or segmentation violation?
  VA  3: 0x0000020b (decimal:  523) --> PA or segmentation violation?
  VA  4: 0x0000019e (decimal:  414) --> PA or segmentation violation?
  VA  5: 0x00000322 (decimal:  802) --> PA or segmentation violation?
  VA  6: 0x00000136 (decimal:  310) --> PA or segmentation violation?
  VA  7: 0x000001e8 (decimal:  488) --> PA or segmentation violation?
  VA  8: 0x00000255 (decimal:  597) --> PA or segmentation violation?
  VA  9: 0x000003a1 (decimal:  929) --> PA or segmentation violation?
```

we just need to set the buond to the highest `VA` + 1 --> 929 + 1 = 930.

```
./relocation.py -s 0 -n 10 -l 930 -c

ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x0000360b (decimal 13835)
  Limit  : 930

Virtual Address Trace
  VA  0: 0x00000308 (decimal:  776) --> VALID: 0x00003913 (decimal: 14611)
  VA  1: 0x000001ae (decimal:  430) --> VALID: 0x000037b9 (decimal: 14265)
  VA  2: 0x00000109 (decimal:  265) --> VALID: 0x00003714 (decimal: 14100)
  VA  3: 0x0000020b (decimal:  523) --> VALID: 0x00003816 (decimal: 14358)
  VA  4: 0x0000019e (decimal:  414) --> VALID: 0x000037a9 (decimal: 14249)
  VA  5: 0x00000322 (decimal:  802) --> VALID: 0x0000392d (decimal: 14637)
  VA  6: 0x00000136 (decimal:  310) --> VALID: 0x00003741 (decimal: 14145)
  VA  7: 0x000001e8 (decimal:  488) --> VALID: 0x000037f3 (decimal: 14323)
  VA  8: 0x00000255 (decimal:  597) --> VALID: 0x00003860 (decimal: 14432)
  VA  9: 0x000003a1 (decimal:  929) --> VALID: 0x000039ac (decimal: 14764)
```
