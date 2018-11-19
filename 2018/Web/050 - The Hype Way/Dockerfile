FROM golang:1.11 AS build-env
COPY main.go go.mod /src/
RUN cd /src && GOOS=js GOARCH=wasm go build -o main.wasm

FROM nginx:1.15-alpine

COPY nginx.conf mime.types /etc/nginx/
COPY index.html wasm_exec.js mini-dark.min.css /usr/share/nginx/html/
COPY --from=build-env /src/main.wasm /usr/share/nginx/html/

RUN chown -R 1001:0 /var/cache/nginx
USER 1001