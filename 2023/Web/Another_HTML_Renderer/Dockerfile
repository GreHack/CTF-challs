FROM alpine:3.18.4

# Init
RUN apk update && \
    apk add --no-cache python3 py3-pip nodejs npm chromium && \
# Create user
    adduser -D -u 1000 challenge && \
    echo "challenge:$(head -c 32 /dev/urandom | base64)" | chpasswd

# Setup challenge
WORKDIR /usr/app/
ADD ./src .
RUN /usr/bin/python3 -m pip install -r requirements.txt;

# Setup bot
WORKDIR /usr/app/bot
RUN npm i

# Switch user
USER 1000:1000

# Start
CMD ["/usr/bin/python3", "/usr/app/app.py"]
