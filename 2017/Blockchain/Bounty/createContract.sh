name=${1%.*}

./compile.sh "$1"
echo "personal.unlockAccount(eth.coinbase, \"\")"
echo "${name}Contract = eth.contract(${name}Info.abi)"
echo "var ${name} = ${name}Contract.new({from:eth.coinbase, data:${name}Info.code, gas:10000000})"

echo "miner.start(8)"
echo "admin.sleepBlocks(2)"
echo "miner.stop()"

echo "tx = eth.getTransactionReceipt(${name}.transactionHash)"
echo "console.log(\"${name} transaction receipt:\")"
echo "inspect(tx)"
echo "${name} = ${name}Contract.at(tx.contractAddress)"
echo "console.log(\"code of the contract: 0x signify an error while setting the contract:\")"
echo "inspect(eth.getCode(${name}.address))"
