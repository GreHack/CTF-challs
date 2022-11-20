# Yaknet 2

## Information to set up the chall on CTFd

_Category_: Network/Reverse

_Points_: 100 (Medium)

_Flag_: `GH22{L4B1RY4TH3_n0CliP_Bl1nD}`

## Description to put on CTFd
YET ANOTHER Karst Net

We developed Yet Another awesome game for people to get lost during GreHack.
You fell in a labyrinth, I heard rumors that there is a flag somewhere.
Find the flag or else you will never see the sunlight again.

I don't know how you managed to get the previous flag, I thought it was impossible.
Did you just cheat? Anyway I modified the server and now you will not be able to pass through walls or holes, again.

I am pretty sure the server side verification is rock solid...
There is no way the player position can be inside a wall/hole.


Author: `Dok & Jeanmi`

## How to access

File: [yaknet](NOPSOURCE)

Usage: ./yaknet [IP] [PORT] (optional:username)

IP ADDRESS : PORT

## Configuration

N/A

## WU

level 2 c'est contournement de l'anti walk-hack

There is a bug inside the movement implementation, you can trick the server if you go to the edge of a block and tp to +0.5 it permit to move to other blocks

  
NO_TP = 1;
NO_WALK_HACK = 1;
NO_FAR_VISION = 0;
