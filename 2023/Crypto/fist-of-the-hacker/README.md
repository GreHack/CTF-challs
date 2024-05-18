# The fist of the hacker

## Description

I always forget my passwords, and passwords managers are insecure, so I decided
to implement an authentication method using behavioral biometrics: I can
authenticate myself using keystroke dynamics just by typing my public
passphrase.
Since keystroke dynamics are proper to each person, you'll never be able to
usurp my identity and recover my flag !

## Flag

`GH{7yp1ng_15_4ll_y0u_n33d}`

## Files

Server side: `server/*`
Client side: `client/*`

Only the client source code should be given to the participants.

## Category

Crypto

## Hardness

5

## Solution

The server returns a parameter `score` for each authentication request.
It can be guessed after several requests that the aim is to minimize the score
in order to authenticate.
Changing little by little the values of a base template one at a time is enough
to do so.

A possible code for the solution is in `solution/solve.py`.
