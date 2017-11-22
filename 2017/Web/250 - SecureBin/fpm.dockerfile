FROM php:7.2-rc-fpm-alpine

COPY src/ /var/www/html/
COPY src/db.sqlite /tmp/
COPY src/encryption.key /tmp/
RUN apk add --no-cache libsodium-dev && docker-php-ext-install sodium
RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf
RUN chown www-data:www-data /tmp/db.sqlite
