FROM composer:1.9 AS builder

COPY src/ /app/
WORKDIR /app

RUN composer install -a

FROM php:7.3-fpm

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf

COPY --from=builder /app/ /var/www/html/src/
COPY app.sq3 /data/
