FROM php:7.3-fpm

RUN pecl install redis-5.1.1 && docker-php-ext-enable redis

COPY public/ /var/www/html/internal-sandbox/public/

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf