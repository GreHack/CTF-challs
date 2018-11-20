FROM nginx:1.15-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY src/public/ /var/www/html/src/public/

RUN chown -R 1001:0 /var/cache/nginx
USER 1001