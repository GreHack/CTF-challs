FROM node:14

RUN apt update && \
    apt install -y psmisc

WORKDIR /usr/src/app

COPY . /usr/src/app/

EXPOSE 3000