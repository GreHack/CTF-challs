FROM golang:1.9
COPY server.go /
RUN CGO_ENABLED=0 GOOS=linux go build -o /server /server.go

FROM alpine:3.6
COPY --from=0 /server /
USER nobody
EXPOSE 8080
CMD ["/server"]
