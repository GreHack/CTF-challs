#!/bin/bash
gcc -static getflag.c -o getflag
docker build -t web_insecure_session_storage .
docker run --name=web_insecure_session_storage --rm -p5000:5000 -it web_insecure_session_storage