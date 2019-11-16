FROM nginx:1.17-alpine

COPY nginx.conf /etc/nginx/nginx.conf

COPY internal-sandbox/public/ /var/www/html/internal-sandbox/public/
COPY default-index/index.html /var/www/html/default/index.html
COPY internal-admin/public/ /var/www/html/internal-admin/public/

RUN chown -R 1001:0 /var/cache/nginx
USER 1001