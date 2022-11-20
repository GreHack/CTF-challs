FROM debian:unstable-slim@sha256:85f3a647c5392d6e4182ec0d501c20270c9723c279dcfbca7fe7240ea001360a AS builder

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get -y install gcc make libsdl2-dev && \
    rm -rf /var/lib/apt/lists/*

COPY src/ .

RUN make

FROM debian:unstable-slim@sha256:85f3a647c5392d6e4182ec0d501c20270c9723c279dcfbca7fe7240ea001360a

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get -y install libsdl2-2.0-0 && \
    rm -rf /var/lib/apt/lists/*

COPY --from=builder /server /server
COPY src/*.qoi /

USER 1000:1000

ENTRYPOINT [ "/server" ]
