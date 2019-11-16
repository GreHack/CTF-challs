FROM node:12 AS builder

COPY src/ /var/www/html/src/

WORKDIR /var/www/html/src/

RUN npm ci && npm run build

FROM nginx:1.17-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY --from=builder /var/www/html/src/dist/ /var/www/html/src/
COPY src/payment.php /var/www/html/src/payment.php

RUN chown -R 1001:0 /var/cache/nginx
USER 1001