FROM golang:1.11 AS build-env
COPY main.go go.mod go.sum /src/
RUN cd /src && CGO_ENABLED=0 go build -o content-parser

FROM alpine:3.8

COPY --from=build-env /src/content-parser /usr/local/bin/content-parser

RUN ls -la /usr/local/bin/

USER 1001

ENTRYPOINT /usr/local/bin/content-parser