FROM nginx:1.17-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY src/ /var/www/html/src/

RUN chown -R 1001:0 /var/cache/nginx
USER 1001