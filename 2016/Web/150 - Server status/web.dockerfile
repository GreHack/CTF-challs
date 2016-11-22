FROM php:7.0-apache

COPY src/ /var/www/html/

RUN a2dismod status
