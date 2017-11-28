personal.unlockAccount(eth.coinbase, "")

// even or odd balance
//eth.sendTransaction({from:eth.coinbase, to:Bounty.address, value: 10000})
eth.sendTransaction({from:eth.coinbase, to:Bounty.address, value: 10001})

miner.start(1)
admin.sleepBlocks(4)
miner.stop()

bountyInitialBalance = eth.getBalance(Bounty.address)
console.log("initial balance of Bounty smart contract is :" + bountyInitialBalance)

hashA="0x037371bf62c8a2c23f8134fc2c7049fbe698860b888c98b955644c4b8f8f3155"

balanceIsOdd = bountyInitialBalance.modulo(2)
console.log("is balance odd: " + balanceIsOdd)
if(balanceIsOdd.equals(1))
{
	console.log("balance is odd, adding one");
	eth.sendTransaction({from:eth.coinbase, to: Bounty.address, value: 1});
	miner.start(8);
	admin.sleepBlocks(2);
	miner.stop();
	bountyInitialBalance = eth.getBalance(Bounty.address);
	console.log("balance is now " + bountyInitialBalance);
}

payed = 2
reward = (bountyInitialBalance.plus(payed))
console.log("reward : ", reward)
Bounty.add_bounty.sendTransaction(reward , hashA, {from:eth.coinbase, value: payed})

miner.start(1)
admin.sleepBlocks(4)
miner.stop()

reward = Bounty.get_reward.call(eth.coinbase)
chall = Bounty.get_chall.call(eth.coinbase)
console.log("chall: " + chall + " reward: (" + reward + ")")

Bounty.bounty_solve.sendTransaction(eth.coinbase, "A", {from:eth.coinbase})
miner.start(1)
admin.sleepBlocks(4)
miner.stop()

Bounty.remove_bounty.sendTransaction({from:eth.coinbase})

miner.start(1)
admin.sleepBlocks(4)
miner.stop()

var win_event = Bounty.Win()
win_event.watch(function(error, result) {
	if(error) {
		console.log("win event error");
		console.log(error);
		return;
	}

	console.log("got Win event !");
});

Bounty.win.sendTransaction({from: eth.coinbase, gas:300000})

miner.start(8)
admin.sleepBlocks(8)
miner.stop()

console.log("final balance of Bounty smart contract is :" + eth.getBalance(Bounty.address))
