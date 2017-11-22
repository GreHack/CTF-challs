FROM nginx:1.12-alpine

COPY nginx.conf /etc/nginx/nginx.conf
COPY src/ /var/www/html/
