#!/bin/bash

sudo docker rm -f privesc3
sudo docker build -t privesc3 .
sudo docker run --name privesc3 --rm -h privesc3 -dit -p 22:22 privesc3