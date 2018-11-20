FROM composer:1.7 AS build-env

COPY composer.json composer.lock /app/
COPY app/ /app/app

RUN composer install --ignore-platform-reqs --no-interaction --no-plugins --no-scripts --no-dev -a

FROM php:7.3-rc-fpm

COPY .env /var/www/html/.env
COPY app/ /var/www/html/app/
COPY bootstrap/ /var/www/html/bootstrap/
COPY public/ /var/www/html/public/
COPY resources/ /var/www/html/resources/
COPY routes/ /var/www/html/routes/
COPY storage/ /var/www/html/storage/
COPY --from=build-env /app/vendor/ /var/www/html/vendor/

RUN mv /var/www/html/public/fake_git/ /var/www/html/public/.git/ && chown -R www-data:www-data /var/www/html/storage/

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf