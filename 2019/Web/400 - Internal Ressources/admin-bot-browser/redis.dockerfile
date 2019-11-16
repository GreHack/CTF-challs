FROM redis:5-alpine

COPY redis.conf /usr/local/etc/redis/redis.conf

USER 1001

CMD [ "redis-server", "/usr/local/etc/redis/redis.conf" ]