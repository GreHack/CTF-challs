FROM golang:1.13 AS builder

COPY main.go go.mod go.sum /app/

WORKDIR /app

RUN go build -o bot-browser

FROM ubuntu:18.04

RUN apt-get update -y \
    && apt-get install -y --no-install-recommends chromium-browser ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/bot-browser /usr/local/bin/

RUN groupadd -r chromium && useradd -r -g chromium -G audio,video chromium \
    && mkdir -p /home/chromium/Downloads && chown -R chromium:chromium /home/chromium

USER chromium

ENTRYPOINT /usr/local/bin/bot-browser