FROM php:7.0-fpm-alpine

COPY image_randomizer/ /var/www/html/
RUN chown -R www-data:www-data /var/www/html/
