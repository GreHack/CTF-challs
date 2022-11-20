FROM rust:1.65.0-alpine3.16@sha256:b846416c0fbe5ce97107e0aba4238b67c3f43ba6b9d9837f6417baa2bd52e01a AS builder

WORKDIR /app
COPY . .

RUN apk add --no-cache musl-dev && cargo build --release

FROM cgr.dev/chainguard/alpine-base:latest@sha256:af4d3c2c6e8398a4f745d317b9ce1353411590f53baa963d476040368c547c86

USER 1000:1000

COPY --from=builder /app/target/release/server /server
COPY --from=builder /app/target/release/healthcheck /healthcheck

HEALTHCHECK CMD /healthcheck

CMD [ "/server" ]
