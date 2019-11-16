FROM php:7.4.0RC5-fpm

COPY public/ /var/www/html/internal-admin/public/

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf