FROM php:7.4.0RC5-fpm

RUN echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf

COPY src/payment.php /var/www/html/src/