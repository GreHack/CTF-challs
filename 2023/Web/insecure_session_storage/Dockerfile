FROM alpine:3.18.4

# Init
RUN apk update && \
    apk add --no-cache python3 py3-pip && \
# Create user
    adduser -D -u 1000 challenge && \
    echo "challenge:$(head -c 32 /dev/urandom | base64)" | chpasswd

# Setup challenge
WORKDIR /usr/app/
ADD ./src .
RUN chmod -R 777 /usr/app/ && \
    /usr/bin/python3 -m pip install -r requirements.txt;

# Setup flag
COPY flag.txt /flag.txt
COPY getflag /getflag
RUN chmod 600 /flag.txt && \
    chmod 4755 /getflag /getflag

# Switch user
USER 1000:1000

# Start
CMD ["/usr/bin/python3", "/usr/app/app.py"]
