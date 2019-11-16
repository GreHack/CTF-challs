FROM nginx:1.17-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY src/index.php src/autocompleter.php src/mini-dark.min.css /var/www/html/src/

RUN chown -R 1001:0 /var/cache/nginx
USER 1001