FROM php:7.4-fpm

RUN apt-get update && apt-get install -y \
    nginx php*-mysql \
    && rm -rf /var/lib/apt/lists/*

RUN docker-php-ext-install mysqli pdo pdo_mysql

COPY nginx/conf/nginx.conf /etc/nginx/conf.d/default.conf

COPY nginx/images/ /var/www/images
COPY nginx/html/ /var/www/html
COPY nginx/admin/ /var/www/admin

RUN chown www-data:www-data -R /var/www/

COPY flag.txt /flag_gg_you_did_it_amaz1ng_4865465468468.txt

EXPOSE 4002

CMD service nginx restart && php-fpm