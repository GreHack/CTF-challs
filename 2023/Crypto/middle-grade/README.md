# Middle grade

# Description

Middle-grade security will meet your privacy requirements perfectly!

# Difficulty - category
Crypto - 6/10 or teenage medium

File to give to players:
- `output.txt`
- `middle-grade.py`

# Solve

Custom 48-bit / 4 rounds symmetric cipher , its possible to make a classical space/time tradeoff (known as meet in the middle) to bruteforce the key in time $2^{24}$ and space $2^{24}$

Solve implemented in `solve.py`, bf runs in 4 mins in a single thread and takes no more than 2-3 GB of ram.

sample output:
```
$ python solve.py
 62%|██████████████████████████████████████████████████████████████████████████████████████████████████████████▋                                                                | 2556/4096 [02:11<01:33, 16.44it/s]U collision key :  9fd, a01, a89, 27e
 62%|██████████████████████████████████████████████████████████████████████████████████████████████████████████▋                                                                | 2557/4096 [02:11<01:19, 19.41it/s]
retrieved key : 9fda01a8927e
flag : b'GH{m33t_t1me_:)}'
```

# Flag

`GH{m33t_t1me_:)}` 



