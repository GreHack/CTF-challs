FROM debian:latest as build
RUN apt-get -y update
RUN apt-get -y install build-essential gcc-multilib
ADD challenge /app
WORKDIR /app
RUN make

FROM scratch
COPY --from=build /app/guessing /app/guessing
COPY --from=build /app/flag.txt /app/flag.txt
COPY --from=build /app/flag2.txt /app/flag2.txt
ENTRYPOINT [ "/app/guessing" ]