# Yaknet
Category: Network/Reverse/Programmation  
Points: 5,   
Authors: dok & jeanmi  

**Yaknet 0**:

> YET ANOTHER Karst Net  
> We developed Yet Another awesome game for people to get lost during GreHack. You fell in a labyrinth, I heard rumors that there is a flag somewhere.  
> Find the flag or else you will never see the sunlight again.  
> This is the first level of the series, test your setup and get the game properly working. You will need the lib SDL2 to run this game.  
> Hint: You can enable debug information by setting the DEBUG environment variable  

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{L4B1RY4TH3_h3ll0_W0rld}</p>
</details>

**Yaknet 1**:

> YET ANOTHER Karst Net  
> We developed Yet Another awesome game for people to get lost during GreHack. You fell in a labyrinth, I heard rumors that there is a flag somewhere.  
> Find the flag or else you will never see the sunlight again.  
> The labyrinth now has holes, you cannot jump over them. I think this level is impossible, you will never get to my flag.
> Hint: You can enable debug information by setting the DEBUG environment variable.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{L4B1RY4TH3_pr0_w4llH4Ck}</p>
</details>

**Yaknet 2**:

> YET ANOTHER Karst Net  
> We developed Yet Another awesome game for people to get lost during GreHack. You fell in a labyrinth, I heard rumors that there is a flag somewhere.  
> Find the flag or else you will never see the sunlight again.  
> I don't know how you managed to get the previous flag, I thought it was impossible. Did you just cheat? 
> Anyway I modified the server and now you will not be able to pass through walls or holes, again.  
> I am pretty sure the server side verification is rock solid... There is no way the player position can be inside a wall/hole.  
> Hint: You can enable debug information by setting the DEBUG environment variable.\r\n\r\nHint 2: Server-side verification differs from the client-side collision. You need to bypass it.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{L4B1RY4TH3_n0CliP_Bl1nD}</p>
</details>

**Yaknet 3**:

> YET ANOTHER Karst Net  
> We developed Yet Another awesome game for people to get lost during GreHack. You fell in a labyrinth, I heard rumors that there is a flag somewhere.   
> Find the flag or else you will never see the sunlight again.  
> This labyrinth is huge and not completed, anyway go find the flag. One does not simply walk into the flag. You will not be able to explore every corners of this labyrinth by walking.  
> Hint: You can enable debug information by setting the DEBUG environment variable.  
> Hint 2: This is a network/programming challenge.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{L4B1RY4TH3_az7ec_4bducti0n}</p>
</details>

**Yaknet 4**:

> YET ANOTHER Karst Net  
> We developed Yet Another awesome game for people to get lost during GreHack. You fell in a labyrinth, I heard rumors that there is a flag somewhere.  
> Find the flag or else you will never see the sunlight again.  
> This level is very very big, a human should be able to find the flag, probably not a sane human. No hacks allowed here.  
> Hint: You can enable debug information by setting the DEBUG environment variable.  
> Hint 2: This is a network/programming challenge.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{L4B1RY4TH3_the_ne7rUnner}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
You can run: ``docker compose up`` inside this directory. You can then access the challenges
with the following IP/port:
* Yaknet 0: `127.0.0.1 port 10000`
* Yaknet 1: `127.0.0.1 port 10001`
* Yaknet 2: `127.0.0.1 port 10002`
* Yaknet 3: `127.0.0.1 port 10003`
* Yaknet 4: `127.0.0.1 port 10004`

A pre-built version of the client is accessible: [src/yaknet.x86_64](src/yaknet.x86_64).
