FROM python:3-alpine
RUN apk add --no-cache socat
COPY tag.py .
COPY secret.py .
COPY launch.sh .
HEALTHCHECK CMD ["timeout", "2", "nc", "localhost", "4361"]
USER 1001:1001
CMD ["sh", "launch.sh"]
