FROM node:18.8-alpine

WORKDIR /home/node/app
COPY "package.json" "/home/node/app/"
RUN yarn install

COPY ["controllers", "/home/node/app/controllers"]
COPY ["routes", "/home/node/app/routes"]
COPY ["types", "/home/node/app/types"]
COPY ["app.ts", "database.ts", "tsconfig.json", "/home/node/app/"]

RUN yarn tsc

USER node

CMD cd /home/node/app && node ./app.js
