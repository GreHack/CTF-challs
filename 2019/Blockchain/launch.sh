# Send transactions to blockchain server at
# `docker run` time
node scripts/deploy_contracts.js 

# Result of previous command is needed to build webpack
node scripts/build.js

npx pushstate-server -d build_webpack &
sh
