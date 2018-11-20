FROM composer:1.7 AS build-env

COPY composer.json composer.lock /app/
COPY src/ /app/src/

RUN composer install --ignore-platform-reqs --no-interaction --no-plugins --no-scripts --no-dev -a

FROM php:7.3-rc-fpm

COPY src/ /var/www/html/src/
COPY --from=build-env /app/vendor/ /var/www/html/vendor/

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf