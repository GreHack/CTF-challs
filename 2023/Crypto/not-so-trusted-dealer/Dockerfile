FROM alpine:latest

# Install socat and python
RUN apk update && apk add socat python3

# Create a group and a user
RUN addgroup -S crypto && adduser -S crypto -G crypto

RUN mkdir /nstd
COPY *.py /nstd
WORKDIR /nstd

USER crypto
CMD socat tcp-l:4242,fork system:"python3 NotSoTrustedDealer_CTFGH23.py",pty,stderr,setsid,sigint,sane
