# Guessing Game

Simple Pown and reverse challenge.

## Build the challenge 

Build the docker for remote host 
```bash
docker buildx build -t guessing .
```

Build the binary for challengers : 
```bash
# Compilation requirements : make gcc and gcc-multilib
cd challenge && make
```

## Start the challenge 

```
docker run -it --rm -p 8080:9001 guessing
```

## Flag info 

There is 3 flags: 
* Reverse : 1 very easy - GH{R3v3rs3_1s_s1mpl3}
* Reverse + pown : 1 easy - GH{1nt3g3r_0v3rfl0w}
* Reverse + scripting : 1 easy - GH{Y0u_Kn0w_scr1pt1ng}

