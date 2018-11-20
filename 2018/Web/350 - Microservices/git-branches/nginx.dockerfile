FROM nginx:1.15-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY public/ /var/www/html/public/

RUN mv /var/www/html/public/fake_git/ /var/www/html/public/.git/ && chown -R 1001:0 /var/cache/nginx
USER 1001