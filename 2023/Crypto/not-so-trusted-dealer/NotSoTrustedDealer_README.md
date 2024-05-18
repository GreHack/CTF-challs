# (Not So) Trusted Dealer

## Description

Alice and Bob want to compute something together, but they don't trust each other.
That's why they agreed to use a special system with a third party to still be able
to achieve it. Indeed, they are trusting you - the so-called Trusted Dealer - to
provide them some auxiliary data for this computation. And you are interested in
their secret inputs. Can you find them ?

`nc ctf.grehack.fr:4242`

The flag is formatted as `GH{alice_secret||bob_secret}` where Alice's and Bob's
secrets are in hexadecimal (`0x` prefix not included).

Created by jellykernel

## Difficulty

8.5/10 : Sour/Umami Hard

## Flag

```
GH{b29a7f740ae34817b3500159fe55415f9bd5a1d7f385f04ddd5108fbcd994912abe96a4fc7ae9622f343d16cc54eea783d262ffce7206531e245ec959a6a5563||80e28081ff5d232dcb99b57cbd91bedb84d39f754557f3fcae4cbeecd4519d46ed7c310fc2a01f908641b19d59a70c0f9738f2732307e358b3a3bea4e7bdcdc5}
```

(Note: the reason why this is so dawn long is that the product of the two prime numbers in the flag is disclosed to the player. I had to use large numbers so that this challenge couldn't be solved just by factorizing the result.)

## Alternative flags (in case 262 characters are too much)

- Lower 256 bits of `alice_secret` and `bob_secret`
```
GH{abe96a4fc7ae9622f343d16cc54eea783d262ffce7206531e245ec959a6a5563||ed7c310fc2a01f908641b19d59a70c0f9738f2732307e358b3a3bea4e7bdcdc5}
```

- Higher 256 bits of `alice_secret` and `bob_secret`
```
GH{b29a7f740ae34817b3500159fe55415f9bd5a1d7f385f04ddd5108fbcd994912||80e28081ff5d232dcb99b57cbd91bedb84d39f754557f3fcae4cbeecd4519d46}
```

## Access

Players will interact with the python program through the network (via docker),
*without* having access to the source code.

## Solve

This challenge is a little particular in the sense that for newcomers or people who
are not familiar with multi-party computation, this challenge can be perceived as being
very hard. But for people who are indeed aware of what is going on, this challenge
can be solved in less than 10 minutes.

In a nutshell, Alice and Bob are two parties, each one holding a secret (that is,
an integer) and they want to perform a computation with these secrets without disclosing
them. That's where multi-party computation (MPC) comes into action. By secret-sharing
their secret in two parts, Alice and Bob can cooperate without having to trust each other.
Generally, addition is well supported and straightforward to do.
However, if they want to multiply their secrets, Alice and Bob have to use a
*Beaver triplet*: a triplet `(lambda, mu, nu)` such as `nu = lambda*mu`.
It can be either provided by a third party (aka. the trusted dealer)
or generated interactively using special protocols.

From there, if x and y are the secrets, and `(lambda, mu, nu)` is a Beaver triplet,
then we have:

```
x*y = (x - lambda + lambda)(y - mu + mu)
   = (d + lambda)(e + mu)
   = d*e + d*mu + e*lambda + lambda*mu
   = d*e + d*mu + e*lambda + nu
```

where `d = x - lambda` and `e = y - mu` are called masks, published beforehand
by Alice and Bob.

In this challenge, the idea is to let an attacker (the player) take control over
the triplet generation, by manually specifying its values (which is basically trust
abuse in our context). Also, the provided implementation (which is NOT a reference
at all) intentionally leaks the mask product `d*e`. (While I could technically disclose
`d` and `e`, this could have make the challenge trivial to solve.) Given that the
computation does not change (players will have to notice that), the attacker can
replay it with different inputs, and finally deduce Alice and Bob's secrets.
In fact, only two queries are needed:

Query 1: enter a valid triplet `(lambda, mu, nu=lambda*nu)` with `mu != lambda`
and `lambda` different from zero. Let's denote the resulting mask product `de1`.

Query 2: enter the triplet `(lambda, lambda, lambda**2)` with the same lambda as
in query 1. Note the second mask product `de2`.

From there, compute the secrets with these formulas:

```
x = (de2 - de1)//(mu - lambda) + lambda
y = (1/lambda)*(x*y - de2 + lambda**2 - lambda*x)
```
(they are derived from the expansion of `d*e` in the two cases.)

I might as well add that the fancy log is not here just to bring some kind
of "realism" to the challenge, it contains useful info for solving the challenge
as well.

A note about why the triplet values are limited to 128 bits: I did that because
otherwise players could just try values as big as they want to lower the mask
product value and hence, get the secrets this way. Although I admit it could be
a valid solution, this is not how it is intended to be solved. (And also so
that people are not sure whether it's Python or C++ which is running behind :D)
