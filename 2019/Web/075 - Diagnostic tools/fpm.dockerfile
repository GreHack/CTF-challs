FROM php:7.4.0RC5-fpm

RUN apt-get update && apt-get install -y iputils-ping iputils-tracepath dnsutils && rm -rf /var/lib/apt/lists/* \
    && echo 'php_flag[display_errors] = off' >> /usr/local/etc/php-fpm.d/www.conf

RUN echo 'GH19{escaping_not_needed}' > /flag

COPY src/ /var/www/html/src/