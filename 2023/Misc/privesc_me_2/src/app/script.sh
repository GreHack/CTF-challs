#!/bin/bash

if [ -z $1 ]
then
   echo "Missing argument :  <environnement : dev|prod|rct|build|prep> expected"
   exit 1
fi

ENV=$1

/opt/app/$ENV-deployment/run.sh