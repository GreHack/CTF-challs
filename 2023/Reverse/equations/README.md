# Equations

## Description

I've hidden my flag, can you find it ?

## Flag

`GH{s1mpl3_l1n34r_4lg3br4}`

## Files

- [equations](equations)

## Category

Reverse

## Hardness

7 -- caramel hard (I'm not so sure about it)

## Solution

The binary first checks that the given flag is of length 25.
Then the flag given by the user is considered as a $25 \times 1$ matrix
$X \in \mathbb{Z}/256\mathbb{Z}$.
It is multiplied by an invertible matrix
$M \in GL\left(\mathbb{Z}/256\mathbb{Z}\right)_{25 \times 25}$, and the
resulting matrix $C = M X$ is compared to the expected result $E$.

In order to recover the flag, one can invert the matrix $M$ and multiply it by
$E$ in order to to obtain the correct flag $F = M^{-1} E$.
