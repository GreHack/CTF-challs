FROM alpine:3.6

COPY bot_browser/bot /

EXPOSE 8080

USER nobody

ENTRYPOINT ["/bot"]
