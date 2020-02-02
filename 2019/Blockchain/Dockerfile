FROM node:10-alpine AS dep_install
# dep for node-gyp
RUN apk add --no-cache make python gcc g++ git
COPY package*.json ./
RUN npm ci --only=production


FROM node:10-alpine AS contract_compiler
WORKDIR /usr/src/app
COPY . .
COPY --from=dep_install node_modules node_modules
RUN npx truffle compile
RUN npx truffle test


# Last stage
FROM node:10-alpine

WORKDIR /usr/src/app
COPY . .
COPY --from=dep_install node_modules node_modules
COPY --from=contract_compiler /usr/src/app/build build
RUN apk add --no-cache curl

HEALTHCHECK --start-period=30s --interval=10s CMD curl -f localhost:9000 || exit 1
COPY ./launch.sh ./launch.sh
CMD ./launch.sh
