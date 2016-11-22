FROM php:7.0-apache

COPY src/ /var/www/html/
COPY src/db/flag.db /db/flag.db
COPY src/db/youtube.db /db/youtube.db

RUN chown -R www-data:www-data /db/

RUN a2enmod rewrite && a2enmod headers
