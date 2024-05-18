FROM zenika/alpine-chrome:115-with-puppeteer

USER root
RUN mkdir /app && chown -R chrome:chrome /app
USER chrome

COPY --chown=chrome:chrome . /app/

WORKDIR /app/

RUN npm i express express-validator

EXPOSE 8000
