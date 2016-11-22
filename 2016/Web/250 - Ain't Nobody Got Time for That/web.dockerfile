FROM nginx:1.11-alpine

COPY nginx.conf /etc/nginx/nginx.conf
COPY image_randomizer/ /var/www/html/
