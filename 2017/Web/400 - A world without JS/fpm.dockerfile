FROM php:7.1-fpm-alpine

COPY src/ /var/www/html/
COPY src/db.sqlite /tmp/
RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf
RUN chown -R www-data:www-data /var/www/html/ && chown www-data:www-data /tmp/db.sqlite
