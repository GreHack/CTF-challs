FROM python:3-alpine

# Install socat and libc6-compat
RUN apk update && \
    apk add --no-cache socat && \
    rm -rf /etc/apk/* # /lib/apk/db/*

# Create a group and user
RUN addgroup -S pwn && adduser -S pwn -G pwn

# Import files from build
WORKDIR /pwn
COPY app.py requirements.txt /pwn/
RUN chmod 555 /pwn/app.py

USER pwn
RUN pip install -r requirements.txt
CMD socat TCP-LISTEN:4242,fork system:"python /pwn/app.py",pty,stderr,echo=0
