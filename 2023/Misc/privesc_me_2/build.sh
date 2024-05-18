#!/bin/bash

sudo docker rm -f privesc2
sudo docker build -t privesc2 .
sudo docker run --name privesc2 --rm -h privesc2 -dit -p 22:22 privesc2