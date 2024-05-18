#!/bin/bash

sudo docker rm -f privesc1
sudo docker build -t privesc1 .
sudo docker run --name privesc1 --rm -h privesc1 -dit -p 22:22 privesc1