FROM python:3.10.7-alpine

RUN apk update && apk add --no-cache socat && rm -rf /etc/apk/*
RUN addgroup -S chall && adduser -S chall -G chall
WORKDIR /chall
COPY server/* ./

USER chall
RUN pip3 --no-cache-dir install numpy
EXPOSE 1234
CMD socat TCP-LISTEN:1234,fork system:"python3 /chall/challenge.py",stderr
