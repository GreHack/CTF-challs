# builder
FROM golang:alpine as builder

RUN apk update && apk add --no-cache git ca-certificates tzdata && update-ca-certificates

RUN adduser -D -g '' appuser

WORKDIR /app

COPY ./go.mod ./go.sum ./

RUN go mod download

COPY ./ .

RUN CGO_ENABLED=0 GOOS=linux go build -o gogopowerrangers .

# alpine runner
FROM alpine:latest  

COPY --from=builder /etc/passwd /etc/passwd

RUN apk --no-cache add ca-certificates

WORKDIR /app

COPY --from=builder /app/static ./static
COPY --from=builder /app/*.html ./
COPY --from=builder /app/.env ./

COPY --from=builder /app/gogopowerrangers .

USER appuser

EXPOSE 5000

CMD ["./gogopowerrangers"]
